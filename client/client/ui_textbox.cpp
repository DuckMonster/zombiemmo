#include "stdafx.h"

#include "ui_textbox.h"

#include "tiletext.h"

ui::CUITextBox::CUITextBox( glm::ivec2 position, glm::ivec2 size, CUIElement * parent ) :
	CUIElement( position, size, parent ),
	m_Foreground( 1.f ), m_Background( 0.f ),
	m_Text( "" ), m_Alignment( Left ),
	m_Drawer( ) {
}

void ui::CUITextBox::SetText( std::string str ) {
	if (m_Text == str)
		return;

	m_Text = str;
	SetDirty( );
}

void ui::CUITextBox::SetAlignment( Alignment align ) {
	m_Alignment = align;
	SetDirty( );
}

void ui::CUITextBox::Render( float delta ) {
	if (IsDirty( ))
		Build( );

	m_Drawer.m_Foreground = m_Foreground;
	m_Drawer.m_Background = m_Background;
	m_Drawer.Render( GetUIPosition( ) );

	CUIElement::Render(delta);
}

void ui::CUITextBox::Build( ) {
	CUIElement::Build();

	int line = 0;

	m_Drawer.Clear( );

	for (size_t i = 0; i < m_Text.size( ); i++) {
		int previousBreak = -1;
		std::vector<glm::ivec2> lineTiles;

		// Line
		for (size_t j = 0; j < m_Text.size( ) - i; j++) {
			char c = m_Text[i + j];

			// Newline
			if (c == '\n') {
				break;
			}

			// Break if line is wider than element
			else if (j >= GetSize( ).x && previousBreak != -1 && line < GetSize( ).y - 1) {
				lineTiles.erase( lineTiles.begin( ) + previousBreak, lineTiles.end( ) );
				break;
			}

			else {
				if (c == ' ')
					previousBreak = j;

				lineTiles.push_back( GetTileChar( c ) );
			}
		}

		PushLine( lineTiles, line );
		line++;
		i += lineTiles.size( );
	}

	Clean( );
}

void ui::CUITextBox::PushLine( std::vector<glm::ivec2>& tiles, int lineIndex ) {
	// Alignment
	float w = ((float)GetSize( ).x - (float)tiles.size( )) / 2.f;
	int x = (int)(w * (int)m_Alignment);

	for (size_t i = 0; i < tiles.size( ); i++) {
		m_Drawer.AddTile( TileData( tiles[i], glm::ivec2( x + i, lineIndex ), color( 1.f ), color( 1.f ) ) );
	}
}