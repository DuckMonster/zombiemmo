#include "stdafx.h"
#include "program.h"
#include "tiletext.h"
#include "config.h"

CProgram::CProgram( CFramework * framework ) :
	m_Framework( framework ), m_Game( NULL ),
	m_Tileset( ) {

	// Load tileset
	glm::ivec2 tilesetSize( Config::Get<int>( "gfx.tileset.width" ), Config::Get<int>( "gfx.tileset.height" ) );
	std::string tilesetFilename = Config::Get<std::string>( "gfx.tileset.filename" );

	m_Tileset.Init( tilesetSize, tilesetFilename );

	CTileset::Active = &m_Tileset;
	m_Network.SetInterface( this );

}

CProgram::~CProgram( ) {
}

void CProgram::Load( ) {
	m_Game = new CGame( this );
	m_Network.Connect( );
}

void CProgram::Update( float delta ) {
	m_Network.Update( delta );
	m_Game->Update( delta );
}

void CProgram::Render( float delta ) {
	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m_Game->Render( delta );
}

void CProgram::KeyEvent( sf::Keyboard::Key key, int state ) {
	m_Game->KeyEvent( key, state );
}

glm::ivec2 CProgram::ConsoleSize( ) {
	return glm::ivec2(
		m_Framework->WindowSize( ).x / m_Tileset.m_Size.x,
		m_Framework->WindowSize( ).y / m_Tileset.m_Size.y ) / Config::Get<int>( "gfx.zoom" );
}

void CProgram::Connected( ) {
	std::cout << "Connected!\n";
}

void CProgram::Packet( const net::packet & pkt ) {
	m_Game->ServerPacket( pkt );
}

void CProgram::Disconnect( ) {
	std::cout << "Disconnected!\n";
	GetFramework( )->Exit( );
}
