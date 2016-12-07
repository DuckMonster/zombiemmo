#include "stdafx.h"

#include "inventory.h"

CInventory::CInventory( ) :
	m_Items( ) {

	// ADD TEMPORARY ITEMS
	m_Items.push_back( Item( "Pistol", "Just some pistol :)", glm::ivec2( 0, 8 ) ) );
	m_Items.push_back( Item( "Piece of shit", "Just a total piece of shit", glm::ivec2( 1, 8 ) ) );
}

void CInventory::AddItem( const Item& item ) {
	m_Items.push_back( item );
}

void CInventory::RemoveItem( const Item& item ) {
	for (int i=0; i < m_Items.size( ); i++) {
		if (m_Items[i].m_Name == item.m_Name) {
			RemoveItem( i );
			break;
		}
	}
}

void CInventory::RemoveItem( const size_t index ) {
	m_Items.erase( m_Items.begin( ) + index );
}