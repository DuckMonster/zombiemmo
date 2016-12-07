#pragma once
#include "map.h"
#include "actor.h"
#include "player.h"
#include "enemy.h"
#include "cameracontroller.h"
#include "shadow.h"
#include "shadowdrawer.h"
#include "uirenderer.h"
#include "ui_inventory.h"
#include "entitycollection.h"
#include "collision.h"
#include "ui_log.h"

class CProgram;

class CGame {
public:
	CGame( CProgram* program );
	~CGame( );

	void Update( float delta );
	void Render( float delta );

	void KeyEvent( sf::Keyboard::Key key, int state );
	void BuildShadows( );

	CPlayer* GetPlayer( ) { return m_LocalPlayerID != -1 ? m_EntityCollection.Get<CPlayer>( m_LocalPlayerID ) : NULL; }

	template<typename T>
	T* SpawnEntity( ) { return SpawnEntity<T>( m_EntityCollection.GetNextID( ) ); }
	template<typename T>
	T* SpawnEntity( entity_id id );
	template<typename T>
	T* SpawnEntity( T* entity );

	CEntity* SpawnEntity( entity::Type type ) { return SpawnEntity( type, m_EntityCollection.GetNextID( ) ); }
	CEntity* SpawnEntity( entity::Type type, entity_id id );

	template<typename T>
	T* GetEntity( entity_id id ) { return dynamic_cast<T*>(GetEntity( id )); }
	CEntity* GetEntity( entity_id id ) { return m_EntityCollection[id]; }
	const std::vector<entity_id>& AllEntites( ) { return m_SpawnedEntities; }

	void DestroyEntity( CEntity* ptr );

	CProgram* const GetProgram( ) { return m_Program; }
	CMap* const GetMap( ) { return &m_Map; }
	ui::CUILog* const GetLog( ) { return &m_Log; }

	CShadowController* GetShadowController( ) { return &m_Shadow; }

	void ServerPacket( const net::packet& pkt );

private:
	CProgram* const			m_Program;
	CMap					m_Map;
	CCameraController		m_Camera;

	CShadowController		m_Shadow;
	CShadowDrawer			m_ShadowDrawer;

	ui::CUILog				m_Log;
	ui::CUITextBox			m_PingTextBox;

	std::vector<entity_id>	m_SpawnedEntities;
	CEntityCollection		m_EntityCollection;
	int						m_LocalPlayerID;

	glt::Texture			m_FrameTexture;
	glt::Texture			m_FrameDepthTexture;
	glt::FrameBuffer		m_FrameBuffer;

	glm::mat4				m_GameMatrix;
	glm::mat4				m_UIMatrix;
};

template<typename T>
inline T * CGame::SpawnEntity( entity_id id ) {
	T* entity = new T( this, id );
	m_EntityCollection.Spawn( id, entity );
	m_SpawnedEntities.push_back( id );

	return entity;
}

template<typename T>
inline T * CGame::SpawnEntity( T * entity ) {
	m_EntityCollection.Spawn( entity->GetID( ), entity );
	return entity;
}

inline CEntity* CGame::SpawnEntity( entity::Type type, entity_id id ) {
	CEntity* entity = NULL;

	switch (type) {
		case entity::Base: entity = SpawnEntity<CEntity>( id ); break;
		//case entity::Actor: entity = SpawnEntity<CActor>( id ); break;
		case entity::Player: entity = SpawnEntity<CPlayer>( id ); break;
		case entity::ItemDrop: entity = SpawnEntity<CItemDrop>( id ); break;

		case entity::Mob: entity = SpawnEntity<CEnemy>( id ); break;
	}

	return entity;
}