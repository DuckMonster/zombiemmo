#pragma once
const int TILE_VERT_SIZE = 2 + 2 + 2 + 2 + 4 + 4;
const int TILE_FACE_SIZE = TILE_VERT_SIZE * 4;

struct TileData {
public:
	glm::ivec2		m_Index;
	glm::ivec2		m_Offset;
	glm::vec4		m_Foreground;
	glm::vec4		m_Background;

	TileData( ) :
		m_Index( 0 ), m_Offset( 0 ),
		m_Foreground( 1.f ), m_Background( 0.f ) {
	}

	TileData( glm::ivec2 index ) :
		m_Index( index ),
		m_Offset( 0 ), m_Foreground( 1 ), m_Background( 0.f ) {
	}

	TileData( glm::ivec2 index, glm::ivec2 offset ) :
		m_Index( index ), m_Offset( offset ),
		m_Foreground( 1.f ), m_Background( 0.f ) {
	}

	TileData( glm::ivec2 index, glm::ivec2 offset, glm::vec4 foreground ) :
		m_Index( index ), m_Offset( offset ), m_Foreground( foreground ),
		m_Background( 0.f ) {
	}

	TileData( glm::ivec2 index, glm::vec4 foreground ) :
		m_Index( index ), m_Foreground( foreground ),
		m_Offset( 0 ), m_Background( 0.f ) {
	}

	TileData( glm::ivec2 index, glm::ivec2 offset, glm::vec4 foreground, glm::vec4 background ) :
		m_Index( index ), m_Offset( offset ),
		m_Foreground( foreground ), m_Background( background ) {
	}
};

class CTileDrawer {
public:
	static glt::Shader& GetShader( );

public:
	CTileDrawer( );
	~CTileDrawer( );

	size_t AddTile( TileData& data );
	void AddTiles( std::vector<TileData>& tiles );

	TileData& GetTile( size_t index ) { return m_TileData[index]; }
	void SetTile( size_t index, TileData data );
	void SetShader( glt::Shader* shader );

	void Clear( );

	void Render( ) { Render( glm::ivec2( 0 ) ); }
	void Render( glm::ivec2 offset );

	size_t GetTileCount( ) { return m_TileData.size( ); }

	void SetDirty( ) { m_Dirty = true; }

	void Build( );
	void Build( size_t index );

	glm::vec4				m_Foreground;
	glm::vec4				m_Background;
	float					m_Depth;
	bool					m_IgnoreDepth;

private:
	void Init( );

	std::array<float, TILE_FACE_SIZE> GetTileData( size_t index );

	glt::VAO				m_VAO;
	glt::VBO<float>			m_VBO;

	glt::Shader*			m_Shader;
	std::vector<TileData>	m_TileData;

	size_t					m_TileCount;
	size_t					m_TileCountMax;

	bool					m_Dirty;
};