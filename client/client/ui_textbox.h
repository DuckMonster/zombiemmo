#pragma once
#include "uitools.h"
#include "ui_element.h"

namespace ui {
	class CUITextBox : public CUIElement {
	public:
		enum Alignment {
			Left = 0,
			Center = 1,
			Right = 2
		};

	public:
		CUITextBox( ) : CUITextBox( glm::ivec2( 0 ), glm::ivec2( 0 ), NULL ) { }
		CUITextBox( glm::ivec2 position, glm::ivec2 size ) : CUITextBox( position, size, NULL ) { }
		CUITextBox( glm::ivec2 position, glm::ivec2 size, CUIElement* parent );

		void SetText( std::string str );
		void SetAlignment( Alignment align );

		void Render( float delta ) override;

		color			m_Foreground, m_Background;

	private:
		void Build( ) override;
		void PushLine( std::vector<glm::ivec2>& tiles, int lineIndex );

		std::string		m_Text;
		Alignment		m_Alignment;

		CTileDrawer		m_Drawer;
	};
}