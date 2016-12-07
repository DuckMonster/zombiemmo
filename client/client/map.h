#pragma once
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
	void Render( float delta );
	void RenderFog( float delta );

	CMapTile GetTile( const glm::ivec2 position );
	void Reveal( const std::vector<glm::ivec2>& coords );

private:
	CGame* const			m_Game;
	CTileDrawer				m_Drawer;
	CTileDrawer				m_FogDrawer;

	glm::ivec2				m_Size;
	std::vector<CMapTile>	m_TileList;
};