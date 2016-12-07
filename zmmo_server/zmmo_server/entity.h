#pragma once
#include "chunk.h"
#include "chunkcomponent.h"
#include "netcomponent.h"
class CGame;

class CEntity {
protected:
	virtual void Packet( protocol::MMO msg, bstream& str );
	virtual void Packet_Spawn( bstream& str );
	virtual void Packet_Position( bstream& str );
	virtual void Packet_Destroy( bstream& str );
	virtual void Packet_Possess( bstream& str );

public:
	CEntity( CGame* game, size_t id );
	virtual ~CEntity( );

	virtual void Update( float delta );

	void Destroy( );
	virtual entity::Type GetType( ) { return entity::Base; }

	glm::ivec2 GetPosition( ) { return m_Position; }
	void SetPosition( glm::ivec2 position, bool sync = true );
	bool AddPosition( glm::ivec2 delta, bool sync = true );

	int GetCollision( ) { return m_Collision; }

	CGame* const GetGame( );
	const entity_id GetID( ) { return m_ID; }

	CChunk* GetChunk( ) { return GetComponent<CChunkCmp>( )->GetChunk( ); }
	virtual void OnEnterChunk( CChunk* chunk, CChunk* previous );

	CNetCmp* GetNet( ) { return GetComponent<CNetCmp>( ); }

	void Send( const protocol::MMO msg );
	void Send( const protocol::MMO msg, player_id player ) { DoSend<player_id>( msg, player ); }
	void Send( const protocol::MMO msg, player_id_list players ) { DoSend<player_id_list>( msg, players ); }

protected:
	template<typename container, typename pktFunc>
	void DoSendFunc( pktFunc func, container& container );
	template<typename ContainerType>
	void DoSend( const protocol::MMO msg, const ContainerType& container );
	template<typename ContainerType>
	void DoSend( const bstream& str, const ContainerType& container );
	void SetCollision( int collision ) { m_Collision = collision; }

	template<typename T>
	T* AddComponent( T* comp );
	template<typename T>
	T* GetComponent( );

	glm::ivec2					m_Position;

private:
	const entity_id				m_ID;
	CGame* const				m_Game;
	int							m_Collision;
	std::vector<CComponent*>	m_Components;
};

template<typename container, typename pktFunc>
inline void CEntity::DoSendFunc( pktFunc func, container & container ) {
	bstream str;
	func( str );
	DoSend( str, container );
}

template<typename ContainerType>
inline void CEntity::DoSend( const protocol::MMO msg, const ContainerType& container ) {
	bstream str;
	Packet( msg, str );
	DoSend( str, container );
}

template<typename ContainerType>
inline void CEntity::DoSend( const bstream & str, const ContainerType & container ) {
	GetGame( )->GetProgram( )->GetNetwork( )->Send( str, container );
}

template<typename T>
inline T* CEntity::AddComponent( T* comp ) {
	m_Components.push_back( comp );
	return comp;
}

template<typename T>
inline T* CEntity::GetComponent( ) {
	for (size_t i = 0; i < m_Components.size( ); i++) {
		T* comp = dynamic_cast<T*>(m_Components[i]);
		if (comp)
			return comp;
	}

	return NULL;
}
