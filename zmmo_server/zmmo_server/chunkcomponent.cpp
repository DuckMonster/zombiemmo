#include "stdafx.h"
#include "chunkcomponent.h"
#include "entity.h"
#include "chunk.h"

CChunkCmp::~CChunkCmp( ) {
	if (GetChunk( ))
		ExitChunk( );
}

void CChunkCmp::UpdatePosition( glm::ivec2 position ) {
	CChunk* chnk = GetEntity( )->GetGame( )->FindChunk( position );
	if (chnk != GetChunk( ))
		EnterChunk( chnk );
}

void CChunkCmp::EnterChunk( CChunk * chunk ) {
	if (GetChunk( ) == chunk)
		return;

	CChunk* previous = GetChunk( );

	if (previous)
		ExitChunk( );

	m_CurrentChunk = chunk;

	if (chunk != NULL) {
		m_CurrentChunk->EntityEnter( GetEntity( ) );
		GetEntity( )->OnEnterChunk( chunk, previous );
	}
}

void CChunkCmp::ExitChunk( ) {
	if (GetChunk( ) == NULL)
		return;

	CChunk* temp = m_CurrentChunk;

	m_CurrentChunk->EntityExit( GetEntity( ) );
	m_CurrentChunk = NULL;

	//GetEntity( )->OnExitChunk( temp );
}