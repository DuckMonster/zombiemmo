#include "stdafx.h"
#include "player.h"

#include "game.h"
#include "itemdrop.h"
#include "chunkmonitorcomponent.h"

CPlayer::CPlayer( CGame * game, size_t id ) : CActor( game, id, 0 ) {
	SetHealth( 5.f );
	AddComponent( new CChunkMonitorCmp( this ) );
}

CPlayer::~CPlayer( ) {
}

void CPlayer::Possess( const player_id player ) {
	m_PlayerID = player;
	Send( protocol::PlayerPossess, player );
}

void CPlayer::OnEnterChunk( CChunk* chunk, CChunk* previous ) {
	CActor::OnEnterChunk( chunk, previous );
	GetComponent<CChunkMonitorCmp>( )->OnEnterChunk( chunk );
}

void CPlayer::PickupItem( CItemDrop * itemEntity ) {
	CActor::PickupItem( itemEntity );
}

void CPlayer::DropItem( const size_t index ) {
	CActor::DropItem( index );
}