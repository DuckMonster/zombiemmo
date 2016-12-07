#include "stdafx.h"
#include "map.h"
#include "game.h"
#include "program.h"

#include "mathutils.hpp"
#include "collision.h"

using namespace glm;

CMap::CMap( CGame * game ) :
	m_Game( game ) {
}

void CMap::LoadMap( std::string filename ) {
	unsigned char* data;
	int width, height, channels;

	data = SOIL_load_image( filename.c_str( ), &width, &height, 0, SOIL_LOAD_RGBA );
	if (data == NULL)
		return;

	m_Size = glm::ivec2( width, height );
	m_TileList.resize( width * height );

	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			int tindex = x + y * width;
			int index = (x + y * width) * 4;
			int color = data[index] << 16 | data[index + 1] << 8 | data[index + 2];

			if (color == 0x000000) {
				m_TileList[tindex] = CMapTile( 0 );
			}
			if (color == 0xFFFFFF) {
				m_TileList[tindex] = CMapTile( collision::Solid | collision::Visibility );
			}
			if (color == 0x00FF00) {
				m_TileList[tindex] = CMapTile( 0 );
			}
			if (color == 0x008000) {
				m_TileList[tindex] = CMapTile( collision::Visibility );
			}
		}
	}
}

void CMap::Update( float delta ) {
}

const glm::ivec2 CMap::RandomEmpty( ) {
	glm::ivec2 pos;

	do {
		pos = random::vrand( glm::ivec2( 0 ), GetSize( ) );
	} while (GetTile( pos ).m_Collision == collision::Solid);

	return pos;
}

CMapTile CMap::GetTile( glm::ivec2 position ) {
	if (position.x < 0 || position.x >= m_Size.x ||
		 position.y < 0 || position.y >= m_Size.y)
		return CMapTile( collision::Solid );

	int index = position.x + position.y * m_Size.x;
	return m_TileList[index];
}