#include "stdafx.h"
#include "game.h"
#include "program.h"

#include "shadow.h"
#include "mathutils.hpp"
#include "itemdrop.h"
#include "mob.h"

CGame::CGame( CProgram * program ) :
	m_Program( program ), m_Map( this ) {

	m_Map.LoadMap( "map.png" );

	for (size_t y = 0; y < 10; y++) {
		for (size_t x = 0; x < 10; x++) {
			glm::ivec2 index( x, y );
			m_ChunkList.push_back( CChunk( this, index, rect( index * 10, glm::ivec2( 10, 10 ) ) ) );
		}
	}

	for (int i=0; i < 100; i++) {
		CEntity* entity = SpawnEntity<CMob>( );
		entity->SetPosition( m_Map.RandomEmpty( ) );
	}
}

CGame::~CGame( ) {
}

void CGame::Update( float delta ) {
	for (int i=0; i < m_ChunkList.size( ); i++)
		m_ChunkList[i].Update( delta );
}

void CGame::PlayerJoin( const player_id id ) {
	CPlayer* player = SpawnEntity<CPlayer>( );

	m_PlayerLookup[id] = player->GetID( );

	player->Send( protocol::EntitySpawn, id );
	player->Possess( id );
	player->SetPosition( m_Map.RandomEmpty( ) );
}

void CGame::PlayerPacket( const player_id id, const net::packet & pkt ) {
	bstream str( pkt.begin( ), pkt.size( ) );

	while (!str.endOfStream( )) {
		protocol::MMO p = (protocol::MMO)str.read<short>( );

		switch (p) {
			case protocol::ActorMove:
				GetPlayerEntity( id )->Move( (direction::Direction)str.read<char>( ) );
				break;

			case protocol::ActorFire:
				GetPlayerEntity( id )->Fire( str.read<cvec2>( ) );
				break;
		}
	}
}

void CGame::PlayerLeave( const player_id id ) {
	player_id_list::iterator i = std::find( m_Players.begin( ), m_Players.end( ), id );

	if (i != m_Players.end( ))
		m_Players.erase( i );

	GetPlayerEntity( id )->Destroy( );
}

void CGame::DestroyEntity( CEntity* ptr ) {
	m_EntityList.Remove( ptr->GetID( ) );
}

CChunk * const CGame::GetChunk( glm::ivec2 index ) {
	if (!rect( glm::ivec2( 0 ), glm::ivec2( 10 ) ).Contains( index ))
		return NULL;

	return &m_ChunkList[index.x + index.y * 10];
}

CChunk* const CGame::FindChunk( glm::ivec2 world ) {
	return GetChunk( world / 10 );
}

std::vector<CChunk*> const CGame::FindChunks( rect & r ) {
	std::vector<CChunk*> result;

	glm::ivec2 min = r.m_Position / 10;
	glm::ivec2 max = (r.m_Position + r.m_Size) / 10;

	glm::ivec2 index;
	for (index.y = min.y; index.y <= max.y; ++index.y) {
		for (index.x = min.x; index.x <= max.x; ++index.x) {
			CChunk* c = GetChunk( index );

			if (c)
				result.push_back( c );
		}
	}

	return result;
}
