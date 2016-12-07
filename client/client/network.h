#pragma once
class NetworkInterface {
public:
	virtual void Connected( ) = 0;
	virtual void Packet( const net::packet& pkt ) = 0;
	virtual void Disconnect( ) = 0;
};

class CNetwork {
public:
	CNetwork( );

	void SetInterface( NetworkInterface* ni );

	void Send( const bstream& stream );
	void Send( const net::packet& pkt );

	int GetPing( ) { return m_Ping; }
	
	void Connect( );
	void Disconnect( );
	void Update( float delta );

private:
	NetworkInterface*	m_Interface;
	net::client			m_Client;

	glt::TimeMark		m_TimeMark;

	int					m_Ping;
};