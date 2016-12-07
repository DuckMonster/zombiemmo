#include "stdafx.h"

#include "tileset.h"

using namespace glm;

namespace {
	float VERT_POSITIONS[] ={
		0.f, 0.f,		0.f, 0.f,
		0.f, 1.f,		0.f, 1.f,
		1.f, 1.f,		1.f, 1.f,
		1.f, 0.f,		1.f, 0.f
	};
}

glt::Shader& CTileDrawer::GetShader( ) {
	static glt::Shader shader;

	if (shader.getHandle( ) == -1)
		shader.compileFile( "shader_tile" );

	return shader;
}

CTileDrawer::CTileDrawer( ) :
	m_Foreground( 1.f ), m_Background( 1.f ), m_Depth( 0.f ), m_IgnoreDepth( false ),
	m_Dirty( false ),
	m_TileCount( 0 ), m_TileCountMax( 0 ) {

	SetShader( &GetShader( ) );
}

CTileDrawer::~CTileDrawer( ) {
}

size_t CTileDrawer::AddTile( TileData& data ) {
	m_TileData.push_back( data );
	SetDirty( );

	return m_TileData.size( ) - 1;
}

void CTileDrawer::AddTiles( std::vector<TileData>& tiles ) {
	m_TileData.insert( m_TileData.end( ), tiles.begin( ), tiles.end( ) );
	SetDirty( );
}

void CTileDrawer::SetTile( size_t index, TileData data ) {
	if (memcmp( &m_TileData[index], &data, sizeof( TileData ) == 0 ))
		return;

	m_TileData[index] = data;

	if (m_Dirty)
		Build( );
	else
		Build( index );
}

void CTileDrawer::SetShader( glt::Shader * shader ) {
	m_Shader = shader;
	Init( );
}

void CTileDrawer::Clear( ) {
	m_TileData.clear( );
	SetDirty( );
}

void CTileDrawer::Render( glm::ivec2 offset ) {
	if (m_Dirty)
		Build( );

	m_VAO.bind( );
	m_Shader->set( "u_tileSize", CTileset::Active->m_Size );
	m_Shader->set( "u_offset", offset );
	m_Shader->set( "u_global.foreground", m_Foreground );
	m_Shader->set( "u_global.background", m_Background );
	m_Shader->set( "u_depth", m_Depth );

	CTileset::Active->m_Texture.bind( );

	if (m_IgnoreDepth)
		glDepthMask( false );

	glDrawArrays( GL_QUADS, 0, 4 * GetTileCount( ) );

	if (m_IgnoreDepth)
		glDepthMask( true );
}

void CTileDrawer::Init( ) {
	m_VAO.bind( );
	m_VBO.bind( );
	m_VBO.dataUsage = GL_STREAM_DRAW;

	GLuint vertexAttrib = m_Shader->getAttrib( "a_vertex" ),
		uvAttrib = m_Shader->getAttrib( "a_uv" ),
		offsetAttrib = m_Shader->getAttrib( "a_offset" ),
		indexAttrib = m_Shader->getAttrib( "a_index" ),
		foregroundAttrib = m_Shader->getAttrib( "a_foreground" ),
		backgroundAttrib = m_Shader->getAttrib( "a_background" );

	glEnableVertexAttribArray( vertexAttrib );
	glEnableVertexAttribArray( uvAttrib );
	glEnableVertexAttribArray( offsetAttrib );
	glEnableVertexAttribArray( indexAttrib );
	glEnableVertexAttribArray( foregroundAttrib );
	glEnableVertexAttribArray( backgroundAttrib );
	glVertexAttribPointer( vertexAttrib, 2, GL_FLOAT, false, TILE_VERT_SIZE * sizeof( float ), 0 );
	glVertexAttribPointer( uvAttrib, 2, GL_FLOAT, false, TILE_VERT_SIZE * sizeof( float ), (void*)(2 * sizeof( float )) );
	glVertexAttribIPointer( offsetAttrib, 2, GL_INT, TILE_VERT_SIZE * sizeof( float ), (void*)(4 * sizeof( float )) );
	glVertexAttribIPointer( indexAttrib, 2, GL_INT, TILE_VERT_SIZE * sizeof( float ), (void*)(6 * sizeof( float )) );
	glVertexAttribPointer( foregroundAttrib, 4, GL_FLOAT, false, TILE_VERT_SIZE * sizeof( float ), (void*)(8 * sizeof( float )) );
	glVertexAttribPointer( backgroundAttrib, 4, GL_FLOAT, false, TILE_VERT_SIZE * sizeof( float ), (void*)(12 * sizeof( float )) );
}

void CTileDrawer::Build( ) {
	m_TileCount = GetTileCount( );
	float* vbo_data = new float[TILE_FACE_SIZE * m_TileCount];

	for (size_t i = 0; i < m_TileCount; i++) {
		int index = TILE_FACE_SIZE * i;

		std::array<float, TILE_FACE_SIZE> data_arr = GetTileData( i );
		memcpy( vbo_data + index, data_arr.data( ), sizeof( data_arr ) );
	}

	if (m_TileCount > m_TileCountMax) {
		m_TileCountMax = m_TileCount;
		m_VBO.setData( vbo_data, TILE_FACE_SIZE * m_TileCount * sizeof( float ) );
	}
	else {
		m_VBO.bind( );
		glBufferSubData( GL_ARRAY_BUFFER, 0, TILE_FACE_SIZE * m_TileCount * sizeof( float ), vbo_data );
	}

	delete[] vbo_data;

	m_Dirty = false;
}

void CTileDrawer::Build( size_t index ) {
	auto arr = GetTileData( index );

	m_VBO.bind( );
	glBufferSubData( GL_ARRAY_BUFFER, index * sizeof( arr ), sizeof( arr ), arr.data( ) );
}

std::array<float, TILE_FACE_SIZE> CTileDrawer::GetTileData( size_t index ) {
	TileData data = GetTile( index );
	std::array<float, TILE_FACE_SIZE> arr;
	float* ptr = arr.data( );

	for (size_t i = 0; i < 4; i++) {
		int index = TILE_VERT_SIZE * i;

		// Position + UV
		memcpy( ptr + index, VERT_POSITIONS + i * 4, 4 * sizeof( float ) );

		// Offset
		memcpy( ptr + index + 4, &data.m_Offset, 2 * sizeof( int ) );

		// Index
		memcpy( ptr + index + 6, &data.m_Index, 2 * sizeof( int ) );

		// Foreground
		memcpy( ptr + index + 8, &data.m_Foreground, 4 * sizeof( float ) );

		// Background
		memcpy( ptr + index + 12, &data.m_Background, 4 * sizeof( float ) );
	}

	int size = sizeof( arr );

	return std::move( arr );
}