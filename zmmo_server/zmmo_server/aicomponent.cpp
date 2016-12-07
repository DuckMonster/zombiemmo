#include "stdafx.h"
#include "aicomponent.h"
#include "game.h"

CAIComponent::CAIComponent( CMob * mob ) :
	CComponent( mob ),
	m_Mob( mob ), m_Target( ),
	m_WalkTimer( random::frand( 0.8f, 1.2f ), random::frand( ) ) {
}

CActor* CAIComponent::GetTarget( ) {
	if (!m_Target)
		return NULL;

	CActor* target = GetGame( )->GetEntity<CActor>( m_Target.GetID( ) );
	if (!target || !target->IsAlive( ))
		ReleaseTarget( );

	return target;
}

void CAIComponent::Update( float delta ) {
	m_WalkTimer.Update( GetTarget( ) ? delta * 2 : delta );

	if (m_WalkTimer.Finished( )) {
		Move( );
		m_WalkTimer.Reset( );
	}
}

void CAIComponent::Move( ) {
	if (GetTarget( ) != NULL) {
		line l( GetEntity( )->GetPosition( ), GetTarget( )->GetPosition( ) );

		if (QueryLineCollision<CEntity>( GetGame( ), l, NULL, collision::Map ))
			ReleaseTarget( );
		else {
			if (math::ilength( GetTarget( )->GetPosition( ) - GetEntity( )->GetPosition( ) ) <= 1)
				GetTarget( )->Hit( 2.f );
			else
				MoveTowards( GetTarget( )->GetPosition( ) );
		}
	}
	else {
		m_Mob->Move( (direction::Direction)random::rand( 0, 3 ) );
		LookForTarget( );
	}
}

void CAIComponent::MoveTowards( glm::ivec2 target ) {
	glm::ivec2 dif = target - GetEntity( )->GetPosition( );
	if (math::ilength( dif ) <= 1)
		return;

	glm::ivec2 max = glm::abs( dif.x ) > glm::abs( dif.y ) ? glm::ivec2( dif.x, 0 ) : glm::ivec2( 0, dif.y );
	glm::ivec2 min = dif - max;

	max = glm::sign( max );
	min = glm::sign( min );

	if (!QueryCollision<CEntity>( GetGame( ), GetEntity( )->GetPosition( ) + max, NULL ))
		m_Mob->Move( math::vecToDir( max ) );
	else if (!QueryCollision<CEntity>( GetGame( ), GetEntity( )->GetPosition( ) + min, NULL ))
		m_Mob->Move( math::vecToDir( min ) );
	else
		m_Mob->Move( math::vecToDir( -min ) );
}

void CAIComponent::LookForTarget( ) {
	CActor* entity = GetGame( )->GetClosestEntity<CPlayer>( GetEntity( )->GetPosition( ), 8, NULL );

	if (entity)
		m_Target = entity;
}

void CAIComponent::ReleaseTarget( ) {
	m_Target = NULL;
}