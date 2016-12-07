#pragma once
#include "uitools.h"
#include "ui_element.h"

namespace ui {
	class CUISprite : public CUIElement {
	public:
		CUISprite( ) : CUISprite( glm::ivec2( 0 ), glm::ivec2( 0 ), NULL ) { }
		CUISprite( glm::ivec2 index ) : CUISprite( index, glm::ivec2( 0 ), NULL ) { }
		CUISprite( glm::ivec2 index, glm::ivec2 position ) : CUISprite( index, position, NULL ) { }
		CUISprite( glm::ivec2 index, glm::ivec2 position, CUIElement* parent );

		void SetIndex( glm::ivec2 index );
		void Render( float delta ) override;

		color				m_Foreground;
		color				m_Background;

	private:
		CTileDrawer			m_Drawer;
	};
}