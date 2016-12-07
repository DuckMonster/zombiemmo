#pragma once
#include "entity.h"
#include "item.h"

class CItemDrop : public CEntity {
public:
	CItemDrop( CGame* game, size_t id );

	void SetItem( const Item& item );
	Item& GetItem( ) { return m_Item; }

private:
	Item	m_Item;
};