#include "stdafx.h"
#include "ui_log.h"

#include "game.h"
#include "program.h"

size_t ui::CUILog::LOG_LENGTH = 5;

ui::CUILog::CUILog( CGame* game ) :
	m_Game( game ) {

	m_Base.SetPosition( glm::ivec2( 0, game->GetProgram( )->ConsoleSize( ).y - 1 ) );
}

ui::CUILog::~CUILog( ) {
	for (Entry* e : m_Entries)
		delete e;

	m_Entries.clear( );
}

void ui::CUILog::Add( std::string msg ) {
	m_Entries.insert( m_Entries.begin( ), new Entry( msg, &m_Base ) );
	while (m_Entries.size( ) > LOG_LENGTH) {
		delete m_Entries[LOG_LENGTH];
		m_Entries.erase( m_Entries.begin( ) + LOG_LENGTH );
	}

	for (int i=0; i < m_Entries.size( ); i++) {
		m_Entries[i]->SetIndex( i );
	}
}

void ui::CUILog::Update( float delta ) {
	for (Entry* e : m_Entries)
		e->Update( delta );
}

void ui::CUILog::Render( float delta ) {
	m_Base.Render( delta );
}

void ui::CUILog::Tick( ) {
	for (Entry* e : m_Entries)
		e->Tick( );
}

ui::CUILog::Entry::Entry( std::string text, CUIElement* base ) :
	m_Tick( 0 ), m_Text( text ), m_TextBox( ) {
	m_TextBox.AttachTo( base );
	m_TextBox.SetText( text );

	m_TextBox.m_Background = glm::vec4( 0.f, 0.f, 0.f, 0.6f );
}

void ui::CUILog::Entry::Update( float update ) {
}

void ui::CUILog::Entry::SetIndex( int index ) {
	m_TextBox.SetPosition( glm::ivec2( 0, -index ) );
}

void ui::CUILog::Entry::Tick( ) {
	m_Tick++;

	if (m_Tick == 0)
		m_TextBox.m_Foreground = color( 1.f );
	if (m_Tick > 0)
		m_TextBox.m_Foreground = HexToColor( 0xa4926b );
	if (m_Tick > 1)
		m_TextBox.m_Foreground = HexToColor( 0x4d3e52 );
}
