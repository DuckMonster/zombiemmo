#include "stdafx.h"
#include "chunkmonitorcomponent.h"

const size_t CChunkMonitorCmp::VISION_RANGE = 2;

CChunkMonitorCmp::~CChunkMonitorCmp( ) {
	for (CChunk* c : m_ChunkList)
		c->PlayerExit( m_Player->GetPlayerID( ) );
}

void CChunkMonitorCmp::OnEnterChunk( CChunk * newOrigin ) {
	rect newRect = rect( newOrigin->GetIndex( ) - glm::ivec2( VISION_RANGE - 1 ), glm::ivec2( VISION_RANGE * 2 - 1 ) );
	std::vector<glm::ivec2> left = m_MonitorRect.Subtract( newRect );
	std::vector<glm::ivec2> entered = newRect.Subtract( m_MonitorRect );

	for (const glm::ivec2& v : left)
		RemoveChunk( GetEntity( )->GetGame( )->GetChunk( v ) );

	for (const glm::ivec2& v : entered)
		AddChunk( GetEntity( )->GetGame( )->GetChunk( v ) );

	m_MonitorRect = newRect;
}

void CChunkMonitorCmp::AddChunk( CChunk* chnk ) {
	if (!chnk)
		return;

	m_ChunkList.push_back( chnk );
	chnk->PlayerEnter( m_Player->GetPlayerID( ) );
}

void CChunkMonitorCmp::RemoveChunk( CChunk* chnk ) {
	if (!chnk)
		return;

	m_ChunkList.erase( std::find( m_ChunkList.begin( ), m_ChunkList.end( ), chnk ) );
	chnk->PlayerExit( m_Player->GetPlayerID( ) );
}
