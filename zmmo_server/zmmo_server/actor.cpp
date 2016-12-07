#include "stdafx.h"
#include "actor.h"
#include "game.h"

#include "itemdrop.h"
#include "projectile.h"

const float CActor::FRICTION = 18.f;

ActorDB& CActor::GetDatabase( ) {
	static ActorDB db( "data/actor.json" );
	return db;
}

CActor::CActor( CGame * game, size_t id, size_t actorID ) :
	CEntity( game, id ),
	m_ActorData( GetDatabase( ).GetData( actorID ) ),
	m_Inventory( ), m_ProjectileList( ),
	m_Health( 0.f ),

	m_Velocity( 0 ), m_PositionBuffer( 0 ) {

	m_ProjectileList.fill( NULL );
}

CActor::~CActor( ) {
	if (m_Controller)
		delete m_Controller;
}

void CActor::Update( float delta ) {
	CEntity::Update( delta );
	UpdateVelocity( delta );

	for (size_t i = 0; i < m_ProjectileList.size( ); i++) {
		if (m_ProjectileList[i] != NULL)
			m_ProjectileList[i]->Update( delta );
	}
}

void CActor::SetController( CActorController* controller ) {
	if (m_Controller)
		delete m_Controller;

	m_Controller = controller;
}

void CActor::UpdateVelocity( float delta ) {
	m_PositionBuffer += m_Velocity * delta;
	m_Velocity -= m_Velocity * FRICTION * delta;

	if (glm::abs( m_PositionBuffer.x ) >= 1.f || glm::abs( m_PositionBuffer.y ) >= 1.f) {
		glm::ivec2 delta = glm::ivec2( m_PositionBuffer );
		AddPosition( delta );
		m_PositionBuffer -= delta;
	}
}

void CActor::SetHealth( float health ) {
	m_Health = health;
	DoSendFunc( std::bind( &CActor::Packet_Health, this, std::placeholders::_1 ), GetNet( )->GetObservers( ) );

	if (!IsAlive( ))
		OnDeath( );
}

void CActor::OnDeath( ) {
	SetCollision( 0 );
}

void CActor::Hit( float damage ) {
	if (!IsAlive( ))
		return;

	SetHealth( m_Health - damage );
}

void CActor::Move( direction::Direction direction ) {
	if (!IsAlive( ))
		return;

	glm::ivec2 delta = math::dirToVec( direction );

	AddPosition( delta, true );
	//DoSendFunc( std::bind( &CActor::Packet_Move, this, std::placeholders::_1, direction ), GetNet( )->GetObservers( ) );

	collision::HitResult<CItemDrop> hitResult;

	if (QueryCollision( GetGame( ), m_Position, &hitResult, collision::Entity, collision::Pickup )) {
		for (CItemDrop* d : hitResult.m_HitList) {
			PickupItem( d );
		}
	}
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

void CActor::Fire( cvec2 target ) {
	if (!IsAlive( ))
		return;

	if (target == cvec2( 0 ))
		return;

	glm::vec2 direction = glm::normalize( glm::vec2( target ) );

	glm::ivec2 hitTarget = GetPosition( ) + glm::ivec2( direction * 10.f );
	size_t id = SpawnProjectile( new CProjectile( this, GetPosition( ), hitTarget ) );

	DoSendFunc( std::bind( &CActor::Packet_Fire, this, std::placeholders::_1, hitTarget, id ), GetNet( )->GetObservers( ) );
}

size_t CActor::SpawnProjectile( CProjectile* p ) {
	for (size_t i = 0; i < m_ProjectileList.size( ); i++) {
		if (m_ProjectileList[i] == NULL || m_ProjectileList[i]->IsDestroyed( )) {
			return SpawnProjectile( p, i );
		}
	}

	return SpawnProjectile( p, 0 );
}

void CActor::ProjectileHit( size_t id, size_t index ) {
	DoSendFunc( std::bind( &CActor::Packet_ProjectileHit, this, std::placeholders::_1, id, index ), GetNet( )->GetObservers( ) );
}

size_t CActor::SpawnProjectile( CProjectile* p, size_t id ) {
	if (m_ProjectileList[id] != NULL)
		delete m_ProjectileList[id];

	m_ProjectileList[id] = p;
	m_ProjectileList[id]->SetID( id );
	return id;
}

void CActor::Packet_Spawn( bstream & str ) {
	CEntity::Packet_Spawn( str );
	str.write<short>( protocol::ActorSpawn );
	str.write<entity_id>( GetID( ) );
	str.write<short>( GetData( ).m_ID );

	Packet_Health( str );
}

void CActor::Packet_Health( bstream & str ) {
	str.write<short>( protocol::ActorHealth );
	str.write<entity_id>( GetID( ) );
	str.write<float>( GetHealth( ) );
}

void CActor::Packet_Move( bstream & str, direction::Direction dir ) {
	str.write<short>( protocol::ActorMove );
	str.write<entity_id>( GetID( ) );
	str.write<char>( dir );
}

void CActor::Packet_Fire( bstream & str, glm::ivec2 target, size_t id ) {
	str.write<short>( protocol::ActorFire );
	str.write<entity_id>( GetID( ) );
	str.write<glm::ivec2>( target );
	str.write<char>( id );
}

void CActor::Packet_ProjectileHit( bstream & str, size_t pIndex, size_t lIndex ) {
	str.write<short>( protocol::ActorProjectileHit );
	str.write<entity_id>( GetID( ) );
	str.write<char>( pIndex );
	str.write<char>( lIndex );
}
