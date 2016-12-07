#include "stdafx.h"
#include "entity.h"
#include "game.h"

#include "collision.h"

using namespace glm;

CEntity::CEntity( CGame* game, size_t id ) :
	m_Game( game ), m_ID( id ),
	m_Collision( collision::Solid | collision::Visibility ) {

	AddComponent( new CChunkCmp( this ) );
	AddComponent( new CNetCmp( this ) );
}

CEntity::~CEntity( ) {
	for (size_t i = 0; i < m_Components.size( ); i++)
		delete m_Components[i];

	m_Components.clear( );
}

void CEntity::Update( float delta ) {
}

void CEntity::Destroy( ) {
	Send( protocol::EntityDestroy );
	GetGame( )->DestroyEntity( this );
}

void CEntity::SetPosition( glm::ivec2 position, bool sync ) {
	m_Position = position;
	GetComponent<CChunkCmp>( )->UpdatePosition( m_Position );

	if (sync)
		Send( protocol::EntityPosition );
}

bool CEntity::AddPosition( glm::ivec2 delta, bool sync ) {
	if (QueryCollision<CEntity>( GetGame( ), GetPosition( ) + delta ))
		return false;

	SetPosition( m_Position + delta, sync );
	return true;
}

CGame* const CEntity::GetGame( ) {
	return m_Game;
}

void CEntity::OnEnterChunk( CChunk* chunk, CChunk* previous ) {
	// Add new observers and remove old ones
	if (chunk != NULL && previous != NULL) {
		GetNet( )->AddDifference( chunk->AllPlayers( ), previous->AllPlayers( ) );
		GetNet( )->RemoveDifference( previous->AllPlayers( ), chunk->AllPlayers( ) );
	}
	// Moving into no chunk, remove all observers
	if (chunk == NULL) {
		GetNet( )->RemoveObserver( previous->AllPlayers( ) );
	}
	// Moving into new, with previous being none, add all observers
	if (previous == NULL) {
		GetNet( )->AddObserver( chunk->AllPlayers( ) );
	}
}

// PACKETS
void CEntity::Send( const protocol::MMO msg ) {
	DoSend( msg, GetNet( )->GetObservers( ) );
}

void CEntity::Packet( protocol::MMO msg, bstream& str ) {
	switch (msg) {
		case protocol::EntitySpawn: Packet_Spawn( str ); Packet_Position( str ); break;
		case protocol::EntityPosition: Packet_Position( str ); break;
		case protocol::EntityDestroy: Packet_Destroy( str ); break;
		case protocol::PlayerPossess: Packet_Possess( str ); break;
	}
}

void CEntity::Packet_Spawn( bstream& str ) {
	str.write<short>( protocol::EntitySpawn );
	str.write<unsigned char>( GetType( ) );
	str.write<entity_id>( GetID( ) );
}

void CEntity::Packet_Position( bstream& str ) {
	str.write<short>( protocol::EntityPosition );
	str.write<entity_id>( GetID( ) );
	str.write<svec2>( svec2( GetPosition( ) ) );
}

void CEntity::Packet_Destroy( bstream& str ) {
	str.write<short>( protocol::EntityDestroy );
	str.write<entity_id>( GetID( ) );
}

void CEntity::Packet_Possess( bstream & str ) {
	str.write<short>( protocol::PlayerPossess );
	str.write<entity_id>( GetID( ) );
}
