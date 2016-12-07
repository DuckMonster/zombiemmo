#pragma once
#include "uitools.h"
#include "ui_element.h"

namespace ui {
	class CUIPane : public CUIElement {
	public:
		CUIPane( ) : CUIPane( glm::ivec2( 0 ), glm::ivec2( 0 ), NULL ) { }
		CUIPane( glm::ivec2 position, glm::ivec2 size ) : CUIPane( position, size, NULL ) { }
		CUIPane( glm::ivec2 position, glm::ivec2 size, CUIElement* parent );

		void Render( float delta ) override;

		color				m_Color;

	private:
		CPrimitiveDrawer	m_PDrawer;
	};
}