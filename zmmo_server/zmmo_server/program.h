#pragma once
#include <net/server.hpp>
#include "framework.h"
#include "game.h"

class CProgram : public ProgramInterface, NetworkInterface {
public:
	CProgram( CFramework* framework );
	~CProgram( );

	void Load( ) override;
	void Update( float delta ) override;
	CFramework* GetFramework( ) { return m_Framework; }
	CNetwork* GetNetwork( ) { return &m_Network; }

private:
	void ClientConnect( const player_id id ) override;
	void ClientPacket( const player_id id, const net::packet& pkt ) override;
	void ClientDisconnect( const player_id id ) override;

	CFramework*		m_Framework;
	CGame*			m_Game;
	CNetwork		m_Network;
};