#include "stdafx.h"
#include "deathdarkness.h"

CDeathDarkness::CDeathDarkness( glm::ivec2 size ) : m_Drawer( ) {
	glm::ivec2 index;

	std::vector<TileData> data;
	data.reserve( size.x * size.y );

	for (int x = -size.x / 2 - 1; x <= size.x / 2; ++x) {
		for (int y = -size.y / 2 - 1; y <= size.y / 2; ++y) {
			float a = glm::sqrt( (float)(x*x + y*y) );
			a = 1.f + (a / 10.f);

			data.push_back( TileData( glm::ivec2( 0 ), glm::ivec2( x, y ), color( 0.f, 0.f, 0.f, a ) ) );
		}
	}

	m_Drawer.AddTiles( data );
}

void CDeathDarkness::Render( glm::ivec2 origin, float scale ) {
	m_Drawer.m_Foreground.a = scale;
	m_Drawer.Render( origin );
}
