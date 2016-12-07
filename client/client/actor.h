#pragma once
#include "entity.h"
#include "inventory.h"
#include "itemdrop.h"
#include "projectile.h"
#include "database.h"

class CActor : public CEntity {
public:
	static ActorDB& GetDatabase( );

public:
	CActor( CGame* game, size_t id, size_t actorID );
	~CActor( );

	const ActorDB::ActorData& GetData( ) { return m_ActorData; }
	CInventory* GetInventory( ) { return &m_Inventory; }

	void Update( float delta ) override;
	void Render( float delta ) override;

	virtual void Move( direction::Direction dir );

	virtual void PickupItem( CItemDrop* itemEntity );
	virtual void DropItem( const size_t index );

	float GetHealth( ) { return m_Health; }
	void SetHealth( float health );
	bool IsAlive( ) { return m_Health > 0.f; }

	virtual void Fire( glm::ivec2 target, size_t projectileIndex );
	void ProjectileHit( size_t id, size_t index );

protected:
	virtual void OnDeath( );
	void SpawnProjectile( CProjectile* p, size_t id );


	float							m_Health;
	float							m_AimAngle;

	float							m_MoveSpeed;
	std::array<CProjectile*, 5>		m_ProjectileList;

private:
	const ActorDB::ActorData		m_ActorData;
	CInventory						m_Inventory;
	Timer							m_HurtTimer;
	CTileDrawer						m_HurtDrawer;
};