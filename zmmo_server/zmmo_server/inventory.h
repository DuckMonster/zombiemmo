#pragma once
#include <vector>
#include "item.h"

class CInventory {
public:
	CInventory( );

	void AddItem( const Item& i );
	void RemoveItem( const Item& i );
	void RemoveItem( const size_t index );
	const std::vector<Item>& GetItems( ) { return m_Items; }
	const size_t GetItemCount( ) { return m_Items.size( ); }
	const Item& GetItem( size_t index ) { return m_Items[index]; }

	const Item& operator[]( size_t index ) { return GetItem( index ); }

private:
	std::vector<Item>	m_Items;
};