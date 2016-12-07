#include "stdafx.h"

#include "uitools.h"

using namespace glm;

glt::Shader& ui::CPrimitiveDrawer::GetShader( ) {
	static glt::Shader shader;

	if (shader.getHandle( ) == -1)
		shader.compileFile( "shader_primitive" );

	return shader;
}

ui::CPrimitiveDrawer::CPrimitiveDrawer( ) {
	Init( );
}

void ui::CPrimitiveDrawer::Quad( glm::ivec2 origin, glm::ivec2 size, color clr ) {
	if (size.x < 0 || size.y < 0 || clr.a <= 0.f)
		return;

	GetShader( ).use( );
	GetShader( ).set( "u_model", GetModelMatrix( origin, size ) );
	GetShader( ).set( "u_material.color", clr );
	m_VAO.bind( );

	glDrawArrays( GL_QUADS, 0, 4 );
}

void ui::CPrimitiveDrawer::String( std::string str, glm::ivec2 offset, color foreground, color background ) {
	m_SDrawer.m_String = str;
	m_SDrawer.m_Foreground = foreground;
	m_SDrawer.m_Background = background;

	m_SDrawer.Render( offset );
}

void ui::CPrimitiveDrawer::Init( ) {
	glt::Shader& shader = GetShader( );

	ivec2 verts[] ={
		ivec2( 0, 0 ),
		ivec2( 0, 1 ),
		ivec2( 1, 1 ),
		ivec2( 1, 0 )
	};

	m_VAO.bind( );
	m_VBO.bind( );
	m_VBO.setData( verts, sizeof( verts ) );

	GLuint a_vertex = shader.getAttrib( "a_vertex" );
	glEnableVertexAttribArray( a_vertex );
	glVertexAttribIPointer( a_vertex, 2, GL_INT, 0, 0 );
}

glm::mat4 ui::CPrimitiveDrawer::GetModelMatrix( glm::ivec2 offset, glm::ivec2 scale ) {
	glm::mat4 m( 1.f );

	m[0][0] = scale.x;
	m[1][1] = scale.y;

	m[3] = vec4( offset, 0.f, 1.f );

	return m;
}
