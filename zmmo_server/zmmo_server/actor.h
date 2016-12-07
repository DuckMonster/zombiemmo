#pragma once
#include "entity.h"
#include "inventory.h"
#include "itemdrop.h"
#include "database.h"

class CProjectile;
class CActorController;

class CActor : public CEntity {
private:
	static const float FRICTION;
	static ActorDB& GetDatabase( );

protected:
	void Packet_Spawn( bstream& str ) override;
	void Packet_Health( bstream& str );
	void Packet_Move( bstream& str, direction::Direction dir );
	void Packet_Fire( bstream& str, glm::ivec2 target, size_t id );
	void Packet_ProjectileHit( bstream& str, size_t id, size_t index );

public:
	CActor( CGame* game, size_t id, size_t actorID );
	~CActor( );

	void Update( float delta ) override;

	entity::Type GetType( ) override { return entity::Actor; }
	const ActorDB::ActorData& GetData( ) { return m_ActorData; }
	CInventory* GetInventory( ) { return &m_Inventory; }

	void SetController( CActorController* controller );
	CActorController* GetController( ) { return m_Controller; }
	template<typename T>
	T* GetController( );

	bool IsAlive( ) { return m_Health > 0.f; }
	float GetHealth( ) { return m_Health; }
	void Hit( float damage );

	void AddForce( glm::vec2 direction, float force ) { m_Velocity += direction * force; }

	virtual void Move( direction::Direction direction );
	virtual void PickupItem( CItemDrop* itemEntity );
	virtual void DropItem( const size_t index );

	void ProjectileHit( size_t id, size_t index );

	void Fire( cvec2 target );

protected:
	void UpdateVelocity( float delta );

	void SetHealth( float health );
	void OnDeath( );

private:
	size_t SpawnProjectile( CProjectile* p );
	size_t SpawnProjectile( CProjectile* p, size_t id );

	CActorController*				m_Controller;

	const ActorDB::ActorData		m_ActorData;
	CInventory						m_Inventory;
	std::array<CProjectile*, 5>		m_ProjectileList;

	float							m_Health;

	glm::vec2						m_Velocity;
	glm::vec2						m_PositionBuffer;
};

template<typename T>
T* CActor::GetController( ) {
	return dynamic_cast<T*>(GetController( ));
}