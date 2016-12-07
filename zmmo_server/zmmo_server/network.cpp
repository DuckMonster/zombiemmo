#include "stdafx.h"

#include "network.h"
#include "lock_break.hpp"

CNetwork::CNetwork( ) {
}

void CNetwork::StartUp( ) {
	short port = Config::Get<int>( "system.port" );

	m_Server.startup( port );
	m_Worker = std::thread( &CNetwork::SendWorker, this );
}

void CNetwork::ShutDown( ) {
	m_Server.shutdown( );
}

void CNetwork::SetInterface( NetworkInterface * ni ) {
	m_Interface = ni;
}

void CNetwork::Update( ) {
	if (!m_Interface)
		return;

	net::event e;
	while (m_Server.pollEvent( e )) {
		switch (e.type( )) {

			case net::eConnect:
				m_Interface->ClientConnect( e.connect( ).id );
				break;

			case net::ePacket: {
				const net::packet& pkt = e.packet( ).pkt;

				// Intercept ping messages
				if (pkt.size( ) == 2) {
					bstream str( pkt.begin( ), pkt.size( ) );

					if (str.read<short>( ) == protocol::Ping)
						Send( pkt, e.packet( ).id );

					continue;
				}
				m_Interface->ClientPacket( e.packet( ).id, e.packet( ).pkt );
			} break;

			case net::eDisconnect:
				m_Interface->ClientDisconnect( e.disconnect( ).id );
				break;

			case net::eError:
				std::cout << "Error: " << e.error( ).message << "\n";
				break;
		}
	}
}

void CNetwork::Send( const bstream & stream, const player_id& id ) {
	Send( net::packet( stream.begin( ), stream.size( ) ), id );
}

void CNetwork::Send( const bstream & stream, const player_id_list& list ) {
	Send( net::packet( stream.begin( ), stream.size( ) ), list );
}

void CNetwork::Send( const net::packet & pkt, const player_id& id ) {
	Send( pkt, &id, 1 );
}

void CNetwork::Send( const net::packet & pkt, const player_id_list& list ) {
	if (list.size( ) <= 0)
		return;

	Send( pkt, &list[0], list.size( ) );
}

void CNetwork::Send( const net::packet& pkt, const player_id* ptr, size_t count ) {
	if (pkt.size( ) == 0)
		return;

	std::lock_guard<std::mutex> lock( m_Mtx );
	m_JobQueue.push( new CPktJob( pkt, ptr, count ) );
	m_JobCV.notify_all( );
}

void CNetwork::SendWorker( ) {
	std::unique_lock<std::mutex> lock( m_Mtx );

	while (Active( )) {
		while (!m_JobQueue.empty( )) {
			CPktJob* job = m_JobQueue.front( );
			m_JobQueue.pop( );

			{
				lock_break brk( lock );
				for (size_t i = 0; i < job->m_Count; i++)
					m_Server.send( job->m_Pkt, job->m_Ptr[i] );

				delete job;
			}
		}

		m_JobCV.wait( lock );
	}
}
