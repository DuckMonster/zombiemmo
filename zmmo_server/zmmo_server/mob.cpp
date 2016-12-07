#include "stdafx.h"
#include "mob.h"

#include "aicomponent.h"

CMob::CMob( CGame * game, entity_id id ) : CActor( game, id, 1 ) {
	SetHealth( 4.f );
	AddComponent( new CAIComponent( this ) );
}

CMob::~CMob( ) {
}

void CMob::Update( float delta ) {
	CActor::Update( delta );

	if (IsAlive( ))
		GetComponent<CAIComponent>( )->Update( delta );
}