#include "stdafx.h"

#include "stringdrawer.h"

#include "tiletext.h"

CStringDrawer::CStringDrawer( ) {
}

void CStringDrawer::Render( glm::ivec2 offset ) {
	if (m_String != m_String_Prev)
		Build( );

	m_Drawer.m_Foreground = m_Foreground;
	m_Drawer.m_Background = m_Background;

	m_Drawer.Render( offset );
}

void CStringDrawer::Build( ) {
	m_Drawer.Clear( );
	m_Drawer.AddTiles( GetTileString( m_String ) );

	m_String_Prev = m_String;
}
