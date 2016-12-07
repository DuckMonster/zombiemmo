#include "stdafx.h"

#include "network.h"
#include "config.h"

namespace {
	float t = 0.f;
}

CNetwork::CNetwork( ) {
}

void CNetwork::SetInterface( NetworkInterface * ni ) {
	m_Interface = ni;
}

void CNetwork::Send( const bstream& stream ) {
	Send( net::packet( stream.begin( ), stream.size( ) ) );
}

void CNetwork::Send( const net::packet& pkt ) {
	m_Client.send( pkt );
}

void CNetwork::Connect( ) {
	std::string ip = Config::Get<std::string>( "system.host.ip" ),
		port = Config::Get<std::string>( "system.host.port" );

	m_Client.connect( { ip, port } );
}

void CNetwork::Disconnect( ) {
	m_Client.disconnect( );
}

void CNetwork::Update( float delta ) {
	t += delta;
	if (t > 1.f) {
		bstream str;
		str.write<short>( protocol::Ping );
		Send( str );

		t = 0.f;
		m_TimeMark = glt::TimeMark( );
	}

	net::event e;
	while (m_Client.pollEvent( e )) {
		switch (e.type( )) {

			case net::eConnect:
				m_Interface->Connected( );
				break;

			case net::ePacket: {
				const net::packet& pkt = e.packet( ).pkt;

				// Intercept ping messages
				if (pkt.size( ) == 2) {
					bstream str( pkt.begin( ), pkt.size( ) );

					if (str.read<short>( ) == protocol::Ping) {
						m_Ping = (m_TimeMark.duration( ) * 1000.f);
						continue;
					}
				}

				m_Interface->Packet( e.packet( ).pkt );
			} break;

			case net::eDisconnect:
				m_Interface->Disconnect( );
				break;
		}
	}
}
