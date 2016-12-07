#pragma once
#include "colorutils.h"

class CStringDrawer {
public:
	CStringDrawer( );

	void Render( glm::ivec2 offset = glm::ivec2( 0 ) );

	std::string		m_String;
	color			m_Foreground;
	color			m_Background;

private:
	void Build( );

	CTileDrawer		m_Drawer;
	std::string		m_String_Prev;
};