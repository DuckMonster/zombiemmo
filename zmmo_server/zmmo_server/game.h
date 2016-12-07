#pragma once
#include "map.h"
#include "entity.h"
#include "actor.h"
#include "player.h"
#include "collection.h"
#include "collision.h"
#include "playercontroller.h"
#include "chunk.h"
#include "line.h"

class CProgram;

class CGame {
public:
	CGame( CProgram* program );
	~CGame( );

	void Update( float delta );

	void PlayerJoin( const player_id id );
	void PlayerPacket( const player_id id, const net::packet& pkt );
	void PlayerLeave( const player_id id );

	template<typename T>
	T* SpawnEntity( );
	void DestroyEntity( CEntity* ptr );

	template<typename T>
	T* GetEntity( size_t id ) { return dynamic_cast<T*>(GetEntity( id )); }
	CEntity* GetEntity( size_t id ) { return m_EntityList[id]; }

	template<typename T>
	T* GetClosestEntity( const glm::ivec2& origin, int radius, T* entity );

	CPlayer* GetPlayerEntity( player_id id ) { return GetEntity<CPlayer>( m_PlayerLookup[id] ); }
	player_id_list GetAllPlayers( ) { return m_Players; }

	CProgram* const GetProgram( ) { return m_Program; }
	CMap* const GetMap( ) { return &m_Map; }

	CChunk* const GetChunk( glm::ivec2 index );
	CChunk* const FindChunk( glm::ivec2 world );
	std::vector<CChunk*> const FindChunks( rect& r );

private:
	CProgram* const					m_Program;
	CMap							m_Map;

	player_id_list					m_Players;
	CCollection<CEntity>			m_EntityList;
	std::array<entity_id, 1024>		m_PlayerLookup;

	std::vector<CChunk>				m_ChunkList;
};

template<typename T>
inline T* CGame::SpawnEntity( ) {
	size_t id = m_EntityList.GetNextID( );

	T* entity = new T( this, id );
	m_EntityList.Add( entity );

	return entity;
}

template<typename T>
inline T* CGame::GetClosestEntity( const glm::ivec2 & origin, int radius, T * entity ) {
	std::vector<CChunk*> chunks = FindChunks( rect( origin - glm::ivec2( radius ), glm::ivec2( radius * 2 + 1 ) ) );

	T* best_Entity = NULL;
	int best_Distance = 0;

	for (size_t i = 0; i < chunks.size( ); i++) {
		const std::vector<entity_id>& entities = chunks[i]->AllEntities( );

		for (size_t j = 0; j < entities.size( ); j++) {
			T* e = GetEntity<T>( entities[j] );
			if (e) {
				int distance = math::ilength( e->GetPosition( ) - origin );

				if (best_Entity == NULL || best_Distance < distance) {
					best_Entity = e;
					best_Distance = distance;
				}
			}
		}
	}

	return best_Entity;
}