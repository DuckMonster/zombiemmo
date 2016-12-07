#pragma once
#include "actor.h"

class CMob : public CActor {
public:
	CMob( CGame* game, entity_id id );
	~CMob( );

	entity::Type GetType( ) override { return entity::Mob; }
	void Update( float delta ) override;
};