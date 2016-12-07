#pragma once
#include "collision.h"

class CChunkCmp;
class CChunkMonitorCmp;
class CEntity;
class CGame;

class CChunk {
	friend class CChunkCmp;
	friend class CChunkMonitorCmp;

public:
	CChunk( CGame* game, const glm::ivec2& index, const rect& rect );

	void Update( float delta );
	const std::vector<entity_id>& AllEntities( ) { return m_EntityList; }
	const std::vector<player_id>& AllPlayers( ) { return m_PlayerList; }

	const glm::ivec2& GetIndex( ) { return m_Index; }
	const rect& GetRect( ) { return m_Rect; }
	CGame* const GetGame( ) { return m_Game; }

private:
	void EntityEnter( CEntity* entity );
	void EntityExit( CEntity* entity );

	void PlayerEnter( player_id id );
	void PlayerExit( player_id id );

	const glm::ivec2		m_Index;

	CGame* const			m_Game;
	const rect				m_Rect;

	std::vector<entity_id>	m_EntityList;
	std::vector<player_id>	m_PlayerList;
};