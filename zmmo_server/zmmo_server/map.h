#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>

class CGame;

class CMapTile {
public:
	int m_Collision;

	CMapTile( ) :
		m_Collision( 0 ) { }
	CMapTile( int collision ) :
		m_Collision( collision ) { }

};

class CMap {
public:
	CMap( CGame* game );

	void LoadMap( std::string filename );
	void Update( float delta );

	const glm::ivec2 RandomEmpty( );
	const glm::ivec2 GetSize( ) { return m_Size; }

	CMapTile GetTile( glm::ivec2 position );

private:
	CGame* const			m_Game;

	glm::ivec2				m_Size;
	std::vector<CMapTile>	m_TileList;
};