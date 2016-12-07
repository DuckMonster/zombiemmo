#pragma once
#include "component.h"
class CEntity;
class CChunk;

class CNetCmp : public CComponent {
public:
	CNetCmp( CEntity* entity ) : CComponent( entity ), m_Observers( ) { }
	~CNetCmp( );

	void AddObserver( const player_id id );
	void AddObserver( const player_id_list& list );
	void RemoveObserver( const player_id id );
	void RemoveObserver( const player_id_list& list );

	void AddDifference( const player_id_list& a, const player_id_list& b );
	void RemoveDifference( const player_id_list& a, const player_id_list& b );

	const player_id_list& GetObservers( ) { return m_Observers; }

private:
	player_id_list	m_Observers;
};