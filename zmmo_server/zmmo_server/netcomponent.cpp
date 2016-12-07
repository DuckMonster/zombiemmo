#include "stdafx.h"
#include "netcomponent.h"

CNetCmp::~CNetCmp( ) {
}

void CNetCmp::AddObserver( const player_id id ) {
	m_Observers.push_back( id );

	GetEntity( )->Send( protocol::EntitySpawn, id );
	//std::cout << GetEntity( )->GetID( ) << " ADDED " << id << " [" << m_Observers.size( ) << "]\n";
}

void CNetCmp::AddObserver( const player_id_list& list ) {
	if (list.empty( ))
		return;

	m_Observers.insert( m_Observers.end( ), list.begin( ), list.end( ) );
	GetEntity( )->Send( protocol::EntitySpawn, list );

	//std::cout << GetEntity( )->GetID( ) << " ADDED [" << m_Observers.size( ) << "]\n";
}

void CNetCmp::RemoveObserver( const player_id id ) {
	auto it = std::find( m_Observers.begin( ), m_Observers.end( ), id );
	if (it != m_Observers.end( ))
		m_Observers.erase( it );

	GetEntity( )->Send( protocol::EntityDestroy, id );
	//std::cout << GetEntity( )->GetID( ) << " REMOVED " << id << " [" << m_Observers.size( ) << "]\n";
}

void CNetCmp::RemoveObserver( const player_id_list& list ) {
	if (list.empty( ))
		return;

	player_id_list temp = m_Observers;
	std::sort( temp.begin( ), temp.end( ) );
	player_id_list::iterator first = m_Observers.begin( );
	player_id_list::iterator last = std::set_difference( temp.begin( ), temp.end( ), list.begin( ), list.end( ), first );

	m_Observers.resize( last - first );
	GetEntity( )->Send( protocol::EntityDestroy, list );

	//std::cout << GetEntity( )->GetID( ) << " REMOVED [" << m_Observers.size( ) << "]\n";
}

void CNetCmp::AddDifference( const player_id_list& a, const player_id_list& b ) {
	player_id_list list( a.size( ) );
	player_id_list::iterator start = list.begin( );
	player_id_list::iterator end = std::set_difference( a.begin( ), a.end( ), b.begin( ), b.end( ), start );

	list.resize( end - start );
	AddObserver( list );
}

void CNetCmp::RemoveDifference( const player_id_list& a, const player_id_list& b ) {
	player_id_list list( a.size( ) );
	player_id_list::iterator start = list.begin( );
	player_id_list::iterator end = std::set_difference( a.begin( ), a.end( ), b.begin( ), b.end( ), start );

	list.resize( end - start );
	RemoveObserver( list );
}
