#include "stdafx.h"
#include "itemdrop.h"

#include "collision.h"

CItemDrop::CItemDrop( CGame * game, size_t id ) :
	CEntity( game, id ), m_Item( "", "", glm::ivec2( 0 ) ) {

	SetCollision( collision::Pickup );
	SetItem( Item( "Pistol", "Just a pistol dude", glm::ivec2( 0, 8 ) ) );
}

void CItemDrop::SetItem( const Item& item ) {
	m_Item = item;
}