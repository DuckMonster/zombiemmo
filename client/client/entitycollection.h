#pragma once
#include "entity.h"

class CEntityCollection {
private:
	typedef std::array<CEntity*, 1024> entity_array;

public:
	CEntityCollection( );

	entity_id GetNextID( );
	entity_id Spawn( CEntity* entity ) { return Spawn( GetNextID( ), entity ); }
	entity_id Spawn( entity_id id, CEntity* entity );
	void Destroy( entity_id id );
	size_t Size( ) { return m_EntityArray.size( ); }

	template<typename T>
	T* Get( entity_id index ) { return dynamic_cast<T*>(m_EntityArray[index]); }

	entity_array::iterator begin( ) { return m_EntityArray.begin( ); }
	entity_array::iterator end( ) { return m_EntityArray.end( ); }

	CEntity* operator[]( entity_id id );

private:
	entity_array m_EntityArray;
};