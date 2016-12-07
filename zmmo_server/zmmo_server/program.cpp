#include "stdafx.h"
#include "program.h"

CProgram::CProgram( CFramework * framework ) :
	m_Framework( framework ), m_Game( NULL ) {

	m_Network.SetInterface( this );
}

CProgram::~CProgram( ) {
}

void CProgram::Load( ) {
	m_Game = new CGame( this );

	m_Network.StartUp( );

	if (m_Network.Active( ))
		std::cout << "Server started at " << Config::Get<int>( "system.port" ) << "!\n";
	else
		std::cout << "Failed to start server at " << Config::Get<int>( "system.port" ) << "!\n";
}

void CProgram::Update( float delta ) {

	m_Network.Update( );
	m_Game->Update( delta );
}

void CProgram::ClientConnect( const player_id id ) {
	std::cout << "Client " << id << " connected!\n";
	m_Game->PlayerJoin( id );
}

void CProgram::ClientPacket( const player_id id, const net::packet & pkt ) {
	m_Game->PlayerPacket( id, pkt );
}

void CProgram::ClientDisconnect( const player_id id ) {
	std::cout << "Client " << id << " disconnected!\n";
	m_Game->PlayerLeave( id );
}
