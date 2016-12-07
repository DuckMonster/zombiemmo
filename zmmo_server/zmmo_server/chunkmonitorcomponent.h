#pragma once
#include "component.h"
#include "player.h"
class CEntity;
class CChunk;

class CChunkMonitorCmp : public CComponent {
public:
	CChunkMonitorCmp( CPlayer* entity ) : CComponent( entity ), m_Player( entity ), m_MonitorRect( ), m_ChunkList( ) { }
	~CChunkMonitorCmp( );

	void OnEnterChunk( CChunk* newOrigin );

private:
	static const size_t VISION_RANGE;

	void AddChunk( CChunk* chnk );
	void RemoveChunk( CChunk* chnk );

	CPlayer* const				m_Player;
	rect						m_MonitorRect;
	std::vector<CChunk*>		m_ChunkList;
};