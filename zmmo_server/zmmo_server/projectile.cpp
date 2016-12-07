#include "stdafx.h"
#include "projectile.h"
#include "actor.h"
#include "collision.h"

CProjectile::CProjectile( CActor* host, glm::ivec2 origin, glm::ivec2 target ) :
	m_Host( host ),
	m_ID( -1 ),
	m_Destroyed( false ),

	m_Line( origin, target ),
	m_Distance( 0.f ),
	m_Time( 0.f ),
	m_Velocity( 200.f ) {

	m_Distance = glm::distance( glm::vec2( origin ), glm::vec2( target ) );
}

void CProjectile::Update( float delta ) {
	if (IsDestroyed( ))
		return;

	int prev = GetLineIndex( );
	m_Time += delta;

	int next = GetLineIndex( );

	if (prev != next)
		HitPosition( next );

	if (GetPercent( ) >= 1.f)
		Destroy( );
}

glm::ivec2 CProjectile::GetPosition( ) {
	int index = (int)((float)(m_Line.GetPoints( ).size( ) - 1) * GetPercent( ));
	return m_Line.GetPoints( )[index];
}

void CProjectile::HitPosition( int index ) {
	glm::ivec2 world = m_Line.GetPoints( )[index];

	collision::HitResult<CActor> result;
	QueryCollision<CActor>( m_Host->GetGame( ), world, &result );

	if (result) {
		if (!result.m_HitList.empty( )) {
			result.m_HitList[0]->Hit( random::frand( 1.f, 2.f ) );
			if (!result.m_HitList[0]->IsAlive( )) {
				result.m_HitList[0]->AddForce( glm::normalize( glm::vec2( m_Line.Second( ) - m_Line.First( ) ) ), random::frand( 40.f, 60.f) );
			}
		}

		Destroy( );
		m_Host->ProjectileHit( GetID( ), index );
	}
}
