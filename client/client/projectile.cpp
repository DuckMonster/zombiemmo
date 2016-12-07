#include "stdafx.h"
#include "projectile.h"
#include "actor.h"
#include "game.h"
#include "collision.h"

CProjectile::CProjectile( CActor* host, glm::ivec2 origin, glm::ivec2 target ) :
	m_Host( host ),
	m_Destroyed( false ),

	m_Line( origin, target ),
	m_Distance( 0.f ),
	m_Time( 0.f ),
	m_Velocity( 200.f ) {

	m_Distance = glm::distance( glm::vec2( origin ), glm::vec2( target ) );

	std::vector<TileData> tiles;
	for (const glm::ivec2& v : m_Line.GetPoints( ))
		tiles.push_back( TileData( glm::ivec2( 0 ), v, color( 1.f, 1.f, 1.f, 0.f ) ) );

	m_Drawer.AddTiles( tiles );
	m_Drawer.m_IgnoreDepth = true;
}

void CProjectile::Update( float delta ) {
	UpdateTiles( delta );

	if (IsDestroyed( ))
		return;

	int prev = GetIndex( );
	m_Time += delta;

	int next = GetIndex( );

	if (prev != next)
		HitPosition( next );

	if (GetPercent( ) >= 1.f)
		Destroy( );
}

void CProjectile::Render( float delta ) {
	m_Drawer.Render( );
}

void CProjectile::Hit( size_t index ) {
	Destroy( );

	for (int i=index; i < m_Drawer.GetTileCount( ); i++) {
		TileData data = m_Drawer.GetTile( i );
		data.m_Foreground.a = 0.f;
		m_Drawer.SetTile( i, data );
	}
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
		Destroy( );
	}

	// Make tile lit [fire]
	TileData data = m_Drawer.GetTile( index );
	data.m_Foreground.a = 1.f;
	m_Drawer.SetTile( index, data );
}

void CProjectile::UpdateTiles( float delta ) {
	for (size_t i = 0; i < m_Drawer.GetTileCount( ); i++) {
		TileData data = m_Drawer.GetTile( i );
		if (data.m_Foreground.a <= 0.001f)
			continue;

		data.m_Foreground.a -= data.m_Foreground.a * delta * 40.f;
		m_Drawer.SetTile( i, data );
	}
}