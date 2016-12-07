#include "stdafx.h"

#include "game.h"
#include "program.h"

#include "shadow.h"
#include "uitools.h"
#include "itemdrop.h"
#include "line.h"
#include "config.h"

using namespace glm;

namespace {
	const ColorGradient PING_COLOR( color( 0.f, 1.f, 0.f, 1.f ), color( 1.f, 0.f, 0.f, 1.f ) );
}

CGame::CGame( CProgram * program ) :
	m_Program( program ), m_Map( this ),
	m_Camera( 1 ),
	m_FrameBuffer( program->GetFramework( )->WindowSize( ).x, program->GetFramework( )->WindowSize( ).y ),
	m_Log( this ), m_LocalPlayerID( -1 ), m_Shadow( this ) {

	m_Map.LoadMap( "map.png" );

	vec2 console = program->ConsoleSize( );

	m_GameMatrix = glm::ortho<float>(
		-console.x / 2, console.x / 2,
		console.y / 2, -console.y / 2,
		-20.f, 20.f );
	m_UIMatrix = glm::ortho<float>(
		0.f, console.x,
		console.y, 0.f,
		-20.f, 20.f );

	m_PingTextBox.SetPosition( glm::ivec2( program->ConsoleSize( ).x - 1, 0 ) );
	m_PingTextBox.SetAlignment( ui::CUITextBox::Right );

	m_FrameDepthTexture.init( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, m_FrameBuffer.getWidth( ), m_FrameBuffer.getHeight( ) );
	m_FrameTexture.init( GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, m_FrameBuffer.getWidth( ), m_FrameBuffer.getHeight( ) );
	m_FrameBuffer.bindTexture( m_FrameTexture, GL_COLOR_ATTACHMENT0 );
	m_FrameBuffer.bindTexture( m_FrameDepthTexture, GL_DEPTH_ATTACHMENT );

	m_Camera.m_Zoom = Config::Get<int>( "gfx.zoom" );

	line l( glm::ivec2( 0 ), glm::ivec2( 10, 5 ) );

	BuildShadows( );
}

CGame::~CGame( ) {
}

void CGame::Update( float delta ) {
	for (const entity_id& id : m_SpawnedEntities)
		GetEntity( id )->Update( delta );
}

void CGame::Render( float delta ) {
	CTileDrawer::GetShader( ).set( "u_camera", m_GameMatrix * m_Camera.GameMatrix( ) );
	CShadowDrawer::GetShader( ).set( "u_camera", m_GameMatrix * m_Camera.GameMatrix( ) );

	m_FrameBuffer.bind( );
	{
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		m_Map.Render( delta );

		for (entity_id i = 0; i < m_SpawnedEntities.size( ); i++) {
			CEntity* entity = GetEntity( m_SpawnedEntities[i] );
			if (!entity)
				continue;

			entity->Render( delta );
		}
		glDisable( GL_DEPTH_TEST );
	}

	m_FrameBuffer.release( );

	m_ShadowDrawer.Render( m_FrameTexture );
	m_Map.RenderFog( delta );

	if (GetPlayer( ))
		GetPlayer( )->RenderGameUI( delta );

	CTileDrawer::GetShader( ).set( "u_camera", m_UIMatrix * m_Camera.UIMatrix( ) );
	ui::CPrimitiveDrawer::GetShader( ).set( "u_camera", m_UIMatrix * m_Camera.UIMatrix( ) );

	if (GetPlayer( ))
		GetPlayer( )->RenderUI( delta );

	int ping = GetProgram( )->GetNetwork( )->GetPing( );
	m_PingTextBox.SetText( "Ping: " + std::to_string( GetProgram( )->GetNetwork( )->GetPing( ) ) );
	m_PingTextBox.m_Foreground = PING_COLOR.Lerp( (float)ping / 100.f );
	m_PingTextBox.m_Background = color( 0.f, 0.f, 0.f, 0.6f );
	m_PingTextBox.Render( delta );

	m_Log.Render( delta );
}

void CGame::KeyEvent( sf::Keyboard::Key key, int state ) {
	if (!GetPlayer( ))
		return;

	if (state == 1)
		m_Log.Tick( );

	GetPlayer( )->HandleInput( key, state );
}

void CGame::BuildShadows( ) {
	if (!GetPlayer( ))
		return;

	m_Shadow.Build( GetPlayer( )->GetPosition( ) );
	m_ShadowDrawer.Build( m_Shadow.GetVisible( ) );
	m_Map.Reveal( m_Shadow.GetVisible( ) );

	for (size_t i = 0; i < m_EntityCollection.Size( ); i++) {
		if (m_EntityCollection[i])
			m_EntityCollection[i]->UpdateVisibility( );
	}
}

void CGame::DestroyEntity( CEntity* ptr ) {
	entity_id id = ptr->GetID( );

	m_EntityCollection.Destroy( id );

	auto it = std::find( m_SpawnedEntities.begin( ), m_SpawnedEntities.end( ), id );
	if (it != m_SpawnedEntities.end( ))
		m_SpawnedEntities.erase( it );
}

void CGame::ServerPacket( const net::packet & pkt ) {
	bstream str( pkt.begin( ), pkt.size( ) );

	while (!str.endOfStream( )) {
		protocol::MMO p = (protocol::MMO)str.read<short>( );

		switch (p) {
			case protocol::EntitySpawn: {
				entity::Type type = (entity::Type)str.read<unsigned char>( );
				short id = str.read<entity_id>( );

				if (id != m_LocalPlayerID)
					CEntity* entity = SpawnEntity( type, id );
			} break;

			case protocol::EntityDestroy: {
				short id = str.read<entity_id>( );
				CEntity* entity = GetEntity( id );

				if (entity != GetPlayer( ))
					entity->Destroy( );
			} break;

			case protocol::EntityPosition: {
				short id = str.read<entity_id>( );
				ivec2 position = str.read<svec2>( );

				GetEntity( id )->SetPosition( position );
			} break;

			case protocol::ActorSpawn: {
				short id = str.read<entity_id>( );
				short aid = str.read<short>( );

				CActor* actor = new CActor( this, id, aid );
				SpawnEntity( actor );
			} break;

			case protocol::ActorHealth: {
				short id = str.read<entity_id>( );
				float health = str.read<float>( );

				GetEntity<CActor>( id )->SetHealth( health );
			} break;

			case protocol::ActorMove: {
				short id = str.read<entity_id>( );
				direction::Direction dir = (direction::Direction)str.read<char>( );

				GetEntity<CActor>( id )->Move( dir );
			} break;

			case protocol::ActorFire: {
				short id = str.read<entity_id>( );
				ivec2 target = str.read<ivec2>( );
				char projID = str.read<char>( );

				GetEntity<CActor>( id )->Fire( target, projID );
			} break;

			case protocol::ActorProjectileHit: {
				short id = str.read<entity_id>( );
				char pId = str.read<char>( );
				char pIndex = str.read<char>( );

				GetEntity<CActor>( id )->ProjectileHit( pId, pIndex );

			} break;

			case protocol::PlayerPossess: {
				short id = str.read<entity_id>( );

				m_LocalPlayerID = id;
				m_Camera.m_Target = GetEntity( id );

				BuildShadows( );

				GetLog( )->Add( "You wake up..." );
			} break;
		}
	}
}
