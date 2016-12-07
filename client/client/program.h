#pragma once
#include "framework.h"
#include "tileset.h"
#include "game.h"
#include "network.h"

class CProgram : public ProgramInterface, NetworkInterface {
public:
	CProgram( CFramework* framework );
	~CProgram( );

	void Load( ) override;
	void Update( float delta ) override;
	void Render( float delta ) override;
	void KeyEvent( sf::Keyboard::Key, int state ) override;

	CNetwork* GetNetwork( ) { return &m_Network; }

	glm::ivec2 ConsoleSize( );
	CFramework* GetFramework( ) { return m_Framework; }
	
private:
	void Connected( ) override;
	void Packet( const net::packet& pkt ) override;
	void Disconnect( ) override;

	CTileset		m_Tileset;
	CFramework*		m_Framework;
	CGame*			m_Game;

	CNetwork		m_Network;
};