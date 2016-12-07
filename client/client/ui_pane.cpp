#include "stdafx.h"

#include "ui_pane.h"

ui::CUIPane::CUIPane( glm::ivec2 position, glm::ivec2 size, CUIElement * parent ) :
	CUIElement( position, size, parent ), m_Color( 0.f ), m_PDrawer( ) {
}

void ui::CUIPane::Render( float delta ) {
	if (!IsVisible( ))
		return;

	if (IsDirty( ))
		Build( );

	m_PDrawer.Quad( GetUIPosition( ), GetSize( ), m_Color );

	CUIElement::Render( delta );
}