#include "stdafx.h"

#include "entitycollection.h"

CEntityCollection::CEntityCollection( ) {
	for (entity_id i = 0; i < m_EntityArray.size( ); i++) {
		m_EntityArray[i] = NULL;
	}
}

entity_id CEntityCollection::GetNextID( ) {
	for (size_t i = 0; i < m_EntityArray.size( ); i++) {
		if (m_EntityArray[i] == NULL)
			return i;
	}

	return -1;
}

entity_id CEntityCollection::Spawn( entity_id id, CEntity * entity ) {
	if (id == -1 || m_EntityArray[id])
		throw std::exception( "Failed to spawn entity" );

	m_EntityArray[id] = entity;
	return id;
}

void CEntityCollection::Destroy( entity_id id ) { 
	if (m_EntityArray[id] == NULL)
		return;

	delete m_EntityArray[id];
	m_EntityArray[id] = NULL;
}

CEntity* CEntityCollection::operator[]( entity_id id ) {
	return m_EntityArray[id];
}
