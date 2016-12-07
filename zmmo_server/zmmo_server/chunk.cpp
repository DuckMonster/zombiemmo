#include "stdafx.h"
#include "chunk.h"

CChunk::CChunk( CGame * game, const glm::ivec2 & index, const rect& rect ) :
	m_Game( game ), m_Index( index ), m_Rect( rect ) {
}

void CChunk::Update( float delta ) {
	for (size_t i = 0; i < m_EntityList.size( ); i++) {
		CEntity* e = GetGame( )->GetEntity( m_EntityList[i] );
		e->Update( delta );
	}
}

void CChunk::EntityEnter( CEntity* entity ) {
	std::vector<entity_id>::iterator it = std::find( m_EntityList.begin( ), m_EntityList.end( ), entity->GetID( ) );
	if (it != m_EntityList.end( ))
		throw std::exception( "This entity is already a member of this chunk." );

	if (entity->GetChunk( ) != NULL)
		entity->GetChunk( )->EntityExit( entity );

	m_EntityList.push_back( entity->GetID( ) );
}

void CChunk::EntityExit( CEntity* entity ) {
	std::vector<entity_id>::iterator it = std::find( m_EntityList.begin( ), m_EntityList.end( ), entity->GetID( ) );
	if (it == m_EntityList.end( ))
		return;

	m_EntityList.erase( it );
}

void CChunk::PlayerEnter( player_id id ) {
	std::vector<player_id>::iterator it = std::find( m_PlayerList.begin( ), m_PlayerList.end( ), id );
	if (it != m_PlayerList.end( ))
		throw std::exception( "This player is already monitoring this chunk." );

	m_PlayerList.push_back( id );
	std::sort( m_PlayerList.begin( ), m_PlayerList.end( ) );

	for (size_t i = 0; i < m_EntityList.size( ); i++) {
		CEntity* entity = GetGame( )->GetEntity( m_EntityList[i] );
		if (entity)
			entity->GetNet( )->AddObserver( id );
	}
}

void CChunk::PlayerExit( player_id id ) {
	std::vector<player_id>::iterator it = std::find( m_PlayerList.begin( ), m_PlayerList.end( ), id );
	if (it == m_PlayerList.end( ))
		return;

	for (size_t i = 0; i < m_EntityList.size( ); i++) {
		CEntity* entity = GetGame( )->GetEntity( m_EntityList[i] );
		if (entity)
			entity->GetNet( )->RemoveObserver( id );
	}

	m_PlayerList.erase( it );
}