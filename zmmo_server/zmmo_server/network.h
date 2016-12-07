#pragma once
typedef std::vector<player_id> player_id_list;

class CPktJob {
public:
	net::packet m_Pkt;
	player_id* m_Ptr;
	size_t m_Count;

	CPktJob( const net::packet& pkt, const player_id* ptr, const size_t count ) :
		m_Pkt( pkt ), m_Ptr( NULL ), m_Count( count ) {

		m_Ptr = new player_id[count];
		memcpy( m_Ptr, ptr, count * sizeof( player_id ) );
	}
	~CPktJob( ) {
		delete[] m_Ptr;
	}
};

class NetworkInterface {
public:
	virtual void ClientConnect( const player_id id ) = 0;
	virtual void ClientPacket( const player_id id, const net::packet& pkt ) = 0;
	virtual void ClientDisconnect( const player_id id ) = 0;
};

class CNetwork {
public:
	CNetwork( );

	void StartUp( );
	void ShutDown( );
	
	void Update( );

	void SetInterface( NetworkInterface* ni );
	bool Active( ) { return m_Server.active( ); }

	void Send( const bstream& stream, const player_id& id );
	void Send( const bstream& stream, const player_id_list& list );
	void Send( const net::packet& pkt, const player_id& id );
	void Send( const net::packet& pkt, const player_id_list& list );

private:
	void Send( const net::packet& pkt, const player_id* ptr, size_t count );
	void SendWorker( );

	NetworkInterface*			m_Interface;
	net::server					m_Server;

	std::thread					m_Worker;
	std::queue<CPktJob*>		m_JobQueue;
	std::mutex					m_Mtx;
	std::condition_variable		m_JobCV;
};