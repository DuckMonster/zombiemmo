#pragma once
#include "uitools.h"
#include "stringdrawer.h"
#include "ui_pane.h"
#include "ui_textbox.h"

namespace ui {
	class UIRenderer {
	public:
		UIRenderer( );

		void Render( float delta );

	private:
		CUIPane				m_Pane1;
		CUIPane				m_Pane2;
		CUITextBox			m_TextBox;
		CUIPane				m_Divider;

		CPrimitiveDrawer	m_PDrawer;
		CStringDrawer		m_SDrawer;
	};
}