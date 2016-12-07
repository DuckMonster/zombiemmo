#pragma once
#include "actor.h"

class CPlayer : public CActor {
public:
	CPlayer( CGame* game, size_t id );
	~CPlayer( );

	void Possess( const player_id player );
	const player_id GetPlayerID( ) { return m_PlayerID; }

	entity::Type GetType( ) override { return entity::Player; }

	void OnEnterChunk( CChunk* chunk, CChunk* previous ) override;
	//void OnExitChunk( CChunk* chunk ) override;

	void PickupItem( CItemDrop* itemEntity ) override;
	void DropItem( const size_t index ) override;

private:
	player_id	m_PlayerID;
};