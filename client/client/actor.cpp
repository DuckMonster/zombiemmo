#include "stdafx.h"
#include "actor.h"
#include "game.h"

#include "tiletext.h"
#include "itemdrop.h"

namespace {
	const ColorGradient DEATH_BG( color( 0.15f, 0.05f, 0.05f, 1.f ), color( 0.3f, 0.1f, 0.1f, 1.f ) );
}

ActorDB & CActor::GetDatabase( ) {
	static ActorDB db( "data/actor.json" );
	return db;
}

CActor::CActor( CGame * game, size_t id, size_t actorID ) :
	CEntity( game, id ),
	m_ProjectileList( ),
	m_MoveSpeed( 10.f ),
	m_ActorData( GetDatabase( ).GetData( actorID ) ) {

	SetSprite( TileData( m_ActorData.m_TileIndex, m_ActorData.m_TileColor ) );

	m_ProjectileList.fill( NULL );

	m_HurtDrawer.AddTile( TileData( glm::ivec2( 0 ), glm::ivec2( 0 ), color( 1.f, 0.f, 0.f, 1.f ) ) );
	m_HurtTimer = Timer( 0.4f, 1.f );
}

CActor::~CActor( ) {
	for (CProjectile* p : m_ProjectileList)
		if (p != NULL)
			delete p;

	m_ProjectileList.fill( NULL );
}

void CActor::Update( float delta ) {
	CEntity::Update( delta );

	m_AimAngle -= m_AimAngle * 5.f * delta;

	for (size_t i = 0; i < m_ProjectileList.size( ); i++) {
		if (m_ProjectileList[i])
			m_ProjectileList[i]->Update( delta );
	}
}

void CActor::Render( float delta ) {
	CEntity::Render( delta );

	for (size_t i = 0; i < m_ProjectileList.size( ); i++) {
		if (m_ProjectileList[i])
			m_ProjectileList[i]->Render( delta );
	}

	if (!m_HurtTimer.Finished( )) {
		m_HurtDrawer.m_Foreground.a = 1.f - m_HurtTimer.m_Value;
		m_HurtDrawer.Render( GetPosition( ) );

		m_HurtTimer.Update( delta );
	}
}

void CActor::Move( direction::Direction dir ) {
	AddPosition( math::dirToVec( dir ) );
}

void CActor::PickupItem( CItemDrop * itemEntity ) {
	GetInventory( )->AddItem( itemEntity->GetItem( ) );
	itemEntity->Destroy( );
}

void CActor::DropItem( size_t index ) {
	CItemDrop* newEntity = GetGame( )->SpawnEntity<CItemDrop>( );
	newEntity->SetPosition( GetPosition( ) );
	newEntity->SetItem( m_Inventory[index] );

	m_Inventory.RemoveItem( index );
}

void CActor::SetHealth( float health ) {
	if (health < m_Health)
		m_HurtTimer.Reset( );

	m_Health = health;

	if (!IsAlive( ))
		OnDeath( );
}

void CActor::Fire( glm::ivec2 target, size_t projectileIndex ) {
	SpawnProjectile( new CProjectile( this, GetPosition( ), target ), projectileIndex );
	m_AimAngle = 35.f;
}

void CActor::ProjectileHit( size_t id, size_t index ) {
	m_ProjectileList[id]->Hit( index );
}

void CActor::OnDeath( ) {
	TileData data = GetDrawer( ).GetTile( 0 );

	data.m_Foreground = color( 0.4f, 0.4f, 0.4f, 1.f );
	data.m_Background = DEATH_BG.Lerp( random::frand( ) );

	GetDrawer( ).SetTile( 0, data );
	GetDrawer( ).m_Depth = -1.f;

	SetCollision( 0 );
}

void CActor::SpawnProjectile( CProjectile* p, size_t id ) {
	if (m_ProjectileList[id] != NULL)
		delete m_ProjectileList[id];

	m_ProjectileList[id] = p;
}
