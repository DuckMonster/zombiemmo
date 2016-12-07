#include "stdafx.h"

#include "ui_sprite.h"

ui::CUISprite::CUISprite( glm::ivec2 index, glm::ivec2 position, CUIElement * parent ) :
	CUIElement( position, glm::ivec2( 1 ), parent ),
	m_Foreground( 1.f ), m_Background( 0.f ) {

	SetIndex( index );
}

void ui::CUISprite::SetIndex( glm::ivec2 index ) {
	m_Drawer.Clear( );
	m_Drawer.AddTile( TileData( index, glm::ivec2( 0 ) ) );
}

void ui::CUISprite::Render( float delta ) {
	if (!IsVisible( ))
		return;

	if (IsDirty( ))
		Build( );

	m_Drawer.m_Foreground = m_Foreground;
	m_Drawer.m_Background = m_Background;
	m_Drawer.Render( GetUIPosition( ) );

	CUIElement::Render( delta );
}
