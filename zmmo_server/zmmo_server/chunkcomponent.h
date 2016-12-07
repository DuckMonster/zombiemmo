#pragma once
#include "component.h"
class CEntity;
class CChunk;

class CChunkCmp : public CComponent {
public:
	CChunkCmp( CEntity* entity ) : CComponent( entity ), m_CurrentChunk( NULL ) { }
	~CChunkCmp( );

	void UpdatePosition( glm::ivec2 position );
	CChunk* const GetChunk( ) { return m_CurrentChunk; }

private:
	void EnterChunk( CChunk* chunk );
	void ExitChunk( );

	CChunk*			m_CurrentChunk;
};