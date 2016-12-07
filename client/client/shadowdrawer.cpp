#include "stdafx.h"

#include "shadowdrawer.h"

#include "colorutils.h"

using namespace glm;

glt::Shader & CShadowDrawer::GetShader( ) {
	static glt::Shader shader;

	if (shader.getHandle( ) == -1)
		shader.compileFile( "shader_shadow" );

	return shader;
}

glt::Shader & CShadowDrawer::GetShadowShader( ) {
	static glt::Shader shader;

	if (shader.getHandle( ) == -1)
		shader.compileFile( "shader_shadow_post" );

	return shader;
}

CShadowDrawer::CShadowDrawer( ) :
	m_vertexCount( 0 ) {
	Init( );
}

void CShadowDrawer::Init( ) {
	// Light
	glt::Shader& shader = GetShader( );

	m_vao.bind( );
	m_vbo.bind( );
	m_vbo.dataUsage = GL_STREAM_DRAW;

	GLuint a_vertex = shader.getAttrib( "a_vertex" );

	glEnableVertexAttribArray( a_vertex );
	glVertexAttribIPointer( a_vertex, 2, GL_INT, 0, 0 );

	// Shadow
	glt::Shader& shadowShader = GetShadowShader( );

	m_shadowVAO.bind( );
	m_shadowVBO.bind( );

	vec2 shadowData[] ={
		vec2( -1.f, -1.f ), vec2( 0.f, 0.f ),
		vec2( 1.f, -1.f ), vec2( 1.f, 0.f ),
		vec2( 1.f, 1.f ), vec2( 1.f, 1.f ),
		vec2( -1.f, 1.f ), vec2( 0.f, 1.f )
	};

	m_shadowVBO.setData( shadowData, sizeof( shadowData ) );

	a_vertex = shadowShader.getAttrib( "a_vertex" );
	GLuint a_uv = shadowShader.getAttrib( "a_uv" );

	glEnableVertexAttribArray( a_vertex );
	glEnableVertexAttribArray( a_uv );
	glVertexAttribPointer( a_vertex, 2, GL_FLOAT, false, 2 * sizeof( vec2 ), 0 );
	glVertexAttribPointer( a_uv, 2, GL_FLOAT, false, 2 * sizeof( vec2 ), (void*)(sizeof( vec2 )) );

	// Shadow tint
	color tintColor = HexToColor( 0x061513 );
	tintColor.a = 0.5f;

	shadowShader.set( "u_tint", tintColor );
}

void CShadowDrawer::Build( const std::vector<ivec2>& hidden ) {
	m_vertexCount = hidden.size( ) * 4;
	ivec2* vbodata = new ivec2[m_vertexCount];

	for (int i=0; i < hidden.size( ); i++) {
		int index = i * 4;

		vbodata[index] = hidden[i];
		vbodata[index + 1] = hidden[i] + ivec2( 0, 1 );
		vbodata[index + 2] = hidden[i] + ivec2( 1, 1 );
		vbodata[index + 3] = hidden[i] + ivec2( 1, 0 );
	}

	m_vbo.setData( vbodata, m_vertexCount * sizeof( ivec2 ) );
}

void CShadowDrawer::Render( glt::Texture& frame ) {
	glt::Stencil::clear( );
	glt::Stencil::start( );
	{
		// Render visibility stencil
		GetShader( ).use( );
		m_vao.bind( );

		glt::Stencil::setColorDraw( false );
		glt::Stencil::func( GL_ALWAYS, 1 );
		glt::Stencil::op( GL_KEEP, GL_KEEP, GL_REPLACE );

		glDrawArrays( GL_QUADS, 0, m_vertexCount );

		glt::Stencil::setColorDraw( true );
		glt::Stencil::func( GL_EQUAL, 0 );
		glt::Stencil::op( GL_KEEP, GL_KEEP, GL_KEEP );
	}

	{
		// Render actual game
		GetShadowShader( ).use( );
		m_shadowVAO.bind( );
		frame.bind( );

		// Hidden stuff
		glt::Stencil::func( GL_EQUAL, 0 );
		GetShadowShader( ).set( "u_grayscale", true );
		glDrawArrays( GL_QUADS, 0, 4 );

		// Shown stuff
		glt::Stencil::func( GL_EQUAL, 1 );
		GetShadowShader( ).set( "u_grayscale", false );
		glDrawArrays( GL_QUADS, 0, 4 );
	}
	glt::Stencil::end( );
}