#include "stdafx.h"

#include "map.h"
#include "game.h"
#include "program.h"

#include "tiletext.h"
#include "collision.h"

using namespace glm;

namespace {
	const ColorGradient COLOR_GROUND( color( 0.1f, 0.1f, 0.1f, 1.f ), color( 0.12f, 0.12f, 0.12f, 1.f ) );
	const ColorGradient COLOR_WALL( color( 0.2f, 0.2f, 0.2f, 1.f ), color( 0.2f, 0.2f, 0.2f, 1.f ) );
	const ColorGradient COLOR_GRASS( HexToColor( 0xa3aa35 ), HexToColor( 0x7faa35 ) );
	const ColorGradient COLOR_GRASS_GROUND( HexToColor( 0x162006 ), HexToColor( 0x21230a ) );
}

CMap::CMap( CGame * game ) :
	m_Game( game ) {
	m_Drawer.m_Depth = -10.f;
}

void CMap::LoadMap( std::string filename ) {
	unsigned char* data;
	int width, height, channels;

	data = SOIL_load_image( filename.c_str( ), &width, &height, 0, SOIL_LOAD_RGBA );
	if (data == NULL)
		return;

	m_Size = ivec2( width, height );
	m_TileList.resize( width * height );

	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			int tindex = x + y * width;
			int index = (x + y * width) * 4;
			int color = data[index] << 16 | data[index + 1] << 8 | data[index + 2];

			if (color == 0x000000) {
				m_Drawer.AddTile( TileData( ivec2( 0, 7 ), ivec2( x, y ), vec4( 0.4f ), COLOR_GROUND.Lerp( random::frand( ) ) ) );
				m_TileList[tindex] = CMapTile( 0 );
			}
			if (color == 0xFFFFFF) {
				m_Drawer.AddTile( TileData( GetTileChar( '#' ), ivec2( x, y ), vec4( 0.74f ), COLOR_WALL.Lerp( random::frand( ) ) ) );
				m_TileList[tindex] = CMapTile( collision::Solid | collision::Visibility );
			}
			if (color == 0x00FF00) {
				m_Drawer.AddTile( TileData( ivec2( random::rand( 4, 10 ), 7 ), ivec2( x, y ), COLOR_GRASS.Lerp( random::frand( ) ), COLOR_GRASS_GROUND.Lerp( random::frand( ) ) ) );
				m_TileList[tindex] = CMapTile( 0 );
			}
			if (color == 0x008000) {
				m_Drawer.AddTile( TileData( ivec2( 1, 7 ), ivec2( x, y ), COLOR_GRASS.Lerp( random::frand( ) ), COLOR_GRASS_GROUND.Lerp( random::frand( ) ) ) );
				m_TileList[tindex] = CMapTile( collision::Visibility );
			}

			m_FogDrawer.AddTile( TileData( ivec2( 0 ), ivec2( x, y ), vec4( 0.f ), vec4( 0.f, 0.f, 0.f, 1.f ) ) );
		}
	}
}

void CMap::Update( float delta ) {
}

void CMap::Render( float delta ) {
	m_Drawer.Render( );
}

void CMap::RenderFog( float delta ) {
	m_FogDrawer.Render( );
}

CMapTile CMap::GetTile( const glm::ivec2 position ) {
	if (position.x < 0 || position.x >= m_Size.x ||
		 position.y < 0 || position.y >= m_Size.y)
		return CMapTile( collision::Solid );

	int index = position.x + position.y * m_Size.x;
	return m_TileList[index];
}

void CMap::Reveal( const std::vector<glm::ivec2>& coords ) {
	for (const glm::ivec2& v : coords) {
		if (v.x < 0 || v.x >= m_Size.x ||
			 v.y < 0 || v.y >= m_Size.y)
			continue;

		int index = v.x + v.y * m_Size.x;
		TileData& data = m_FogDrawer.GetTile( index );

		if (data.m_Background.a == 0.f)
			continue;

		data.m_Background.a = 0.f;
		m_FogDrawer.Build( index );
	}
}
