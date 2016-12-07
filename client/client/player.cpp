#include "stdafx.h"

#include "player.h"

#include "game.h"
#include "program.h"

#include "itemdrop.h"
#include "cone.h"

using namespace glm;

const float CPlayer::INPUT_REPEAT_SPEED = 14.f;

CPlayer::CPlayer( CGame * game, size_t id ) :
	CActor( game, id, 0 ),
	m_Mode( ENone ),
	m_CursorPosition( 0 ),
	m_CursorDrawer( ),
	m_AimDrawer( ),

	m_DeathAnimation( 4.f ),
	m_DeathDarkness( game->GetProgram( )->ConsoleSize( ) / 2 ) {

	m_InputController.SetReceiver( this );
	m_CursorDrawer.AddTile( TileData( ivec2( 8, 0 ) ) );
}

void CPlayer::Move( direction::Direction dir ) {
	CActor::Move( dir );
	GetGame( )->BuildShadows( );
}

void CPlayer::HandleInput( sf::Keyboard::Key key, int state ) {
	switch (key) {
		case sf::Keyboard::Up:
		case sf::Keyboard::Right:
		case sf::Keyboard::Left:
		case sf::Keyboard::Down: state ? m_InputController.AddInput( key ) : m_InputController.ReleaseInput( key ); break;
		default: state ? InputTick( key ) : InputReleased( key ); break;
	}
}

void CPlayer::InputTick( sf::Keyboard::Key k ) {
	switch (m_Mode) {
		// AIMING
		case EAiming: {
			switch (k) {
				case sf::Keyboard::Up: --m_CursorPosition.y; break;
				case sf::Keyboard::Right: ++m_CursorPosition.x; break;
				case sf::Keyboard::Left: --m_CursorPosition.x; break;
				case sf::Keyboard::Down: ++m_CursorPosition.y; break;

				case sf::Keyboard::F: SendFire( m_CursorPosition ); break;

				case sf::Keyboard::Escape:
				case sf::Keyboard::A: SetMode( ENone ); break;
			}
		} break;

			// INVENTORY
		case EInventory: {
			switch (k) {
				case sf::Keyboard::Down: m_InventoryUI.ScrollSelected( 1 ); break;
				case sf::Keyboard::Up: m_InventoryUI.ScrollSelected( -1 ); break;

				case sf::Keyboard::D: DropItem( m_InventoryUI.GetSelected( ) ); break;

				case sf::Keyboard::Escape:
				case sf::Keyboard::I: CloseInventory( ); break;
			}
		}break;

			// EXITING
		case EExiting: {
			switch (k) {
				case sf::Keyboard::Y: GetGame( )->GetProgram( )->GetFramework( )->Exit( );

				default: SetMode( ENone ); break;
			}
		} break;

			// NONE
		default: {
			switch (k) {
				case sf::Keyboard::Up: SendMove( direction::North ); break;
				case sf::Keyboard::Right: SendMove( direction::East ); break;
				case sf::Keyboard::Left: SendMove( direction::West ); break;
				case sf::Keyboard::Down: SendMove( direction::South ); break;

				case sf::Keyboard::I: OpenInventory( ); break;
				case sf::Keyboard::A: SetMode( EAiming ); break;
				case sf::Keyboard::E: SetHealth( 0 ); break;

				case sf::Keyboard::Escape: SetMode( EExiting ); break;
			}
		} break;
	}
}

void CPlayer::InputReleased( sf::Keyboard::Key k ) {
}

float CPlayer::GetInputTickFrequency( ) {
	switch (m_Mode) {
		case ENone: return m_MoveSpeed; break;
		default: return INPUT_REPEAT_SPEED; break;
	}
}

void CPlayer::PickupItem( CItemDrop * itemEntity ) {
	GetGame( )->GetLog( )->Add( "You picked up " + itemEntity->GetItem( ).m_Name + "!" );

	CActor::PickupItem( itemEntity );

	m_InventoryUI.Build( GetInventory( ) );
}

void CPlayer::DropItem( const size_t index ) {
	GetGame( )->GetLog( )->Add( "You dropped " + GetInventory( )->GetItem( index ).m_Name + "!" );

	CActor::DropItem( index );

	int m_Selected = m_InventoryUI.GetSelected( );
	m_InventoryUI.Build( GetInventory( ) );

	if (m_Selected >= GetInventory( )->GetItemCount( ))
		m_InventoryUI.Select( GetInventory( )->GetItemCount( ) - 1 );
	else
		m_InventoryUI.Select( m_Selected );
}

void CPlayer::SetPosition( ivec2 position ) {
	CActor::SetPosition( position );

	if (GetGame( )->GetPlayer( ) == this)
		GetGame( )->BuildShadows( );
}

void CPlayer::Update( float delta ) {
	CActor::Update( delta );

	if (IsAlive( ))
		m_InputController.Update( delta );
}

void CPlayer::Render( float delta ) {
	CActor::Render( delta );
}

void CPlayer::RenderGameUI( float delta ) {
	if (!IsAlive( )) {
		m_DeathAnimation.Update( delta );
		if (m_DeathAnimation.Finished( ))
			GetGame( )->GetProgram( )->GetFramework( )->Exit( );

		m_DeathDarkness.Render( GetPosition( ), m_DeathAnimation.Sub( 0.f, 2.f ) );

		GetDrawer( ).m_Foreground.a = GetDrawer( ).m_Background.a = 1.f - m_DeathAnimation.Sub( 2.f, 2.f );
		GetDrawer( ).Render( GetPosition( ) );

		return;
	}

	if (m_Mode == EAiming) {
		BuildAim( );
		m_AimDrawer.Render( );
		m_CursorDrawer.Render( GetPosition( ) + m_CursorPosition );
	}
}

void CPlayer::RenderUI( float delta ) {
	if (!IsAlive( ))
		return;

	if (m_Mode == EInventory)
		m_InventoryUI.Render( delta );
}

bool CPlayer::IsLocal( ) { return GetGame( )->GetPlayer( ) == this; }

void CPlayer::OpenInventory( ) {
	m_InventoryUI.Build( GetInventory( ) );
	m_InventoryUI.Select( 0 );

	SetMode( EInventory );
}

void CPlayer::CloseInventory( ) {
	SetMode( ENone );
}

void CPlayer::SendMove( direction::Direction dir ) {
	bstream str;

	str.write<short>( protocol::ActorMove );
	str.write<char>( dir );

	GetGame( )->GetProgram( )->GetNetwork( )->Send( str );
}

void CPlayer::SendFire( glm::ivec2 target ) {
	bstream str;

	str.write<short>( protocol::ActorFire );
	str.write<cvec2>( target );

	GetGame( )->GetProgram( )->GetNetwork( )->Send( str );
}

void CPlayer::SetMode( Mode mode ) {
	if (mode == EAiming) {
		m_CursorPosition = ivec2( 0 );
		BuildAim( );
	}
	if (mode == EExiting) {
		GetGame( )->GetLog( )->Add( "Exit game? (Y/N)" );
	}

	if (mode == ENone) {
		if (m_Mode == EExiting)
			GetGame( )->GetLog( )->Add( "Okay." );
	}

	m_Mode = mode;
}

void CPlayer::BuildAim( ) {
	m_AimDrawer.Clear( );

	if (m_CursorPosition == ivec2( 0 ))
		return;

	vec2 direction = AimDirection( );
	ivec2 target = ivec2( direction * 10.f );

	line l( GetPosition( ), GetPosition( ) + target );

	// Check for line collisions
	collision::HitResult<CEntity> result;
	QueryLineCollision( GetGame( ), l, &result );

	if (result) {
		color clr = result.m_HitList.size( ) > 0 ? color( 1.f, 0.f, 0.f, 0.6f ) : color( 1.f, 1.f, 1.f, 0.6f );
		m_AimDrawer.AddTile( TileData( ivec2( 9, 0 ), result.m_Point, clr ) );

		l = line( GetPosition( ), result.m_Point );
	}

	// Add data
	std::vector<TileData> data;
	const std::vector<ivec2> points = l.GetPoints( );

	for (size_t i = 1; i < points.size( ); i++) {
		data.push_back( TileData( ivec2( 0 ), points[i], color( 1.f, 1.f, 1.f, 0.1f ) ) );
	}

	m_AimDrawer.AddTiles( data );
}

void CPlayer::OnDeath( ) {
	CActor::OnDeath( );

	if (IsLocal( )) {
		GetGame( )->GetLog( )->Add( "You died." );
		SetMode( ENone );
	}
}