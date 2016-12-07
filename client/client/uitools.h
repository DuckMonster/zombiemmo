#pragma once
#include "colorutils.h"
#include "stringdrawer.h"

namespace ui {
	class CPrimitiveDrawer {
	public:
		static glt::Shader& GetShader( );

	public:
		CPrimitiveDrawer( );

		void Quad( glm::ivec2 origin, glm::ivec2 size, color clr = color( 1.f ) );
		void String( std::string str, glm::ivec2 offset, color foreground = color( 1.f ), color background = color( 1.f ) );

	private:
		void Init( );
		void InitLine( const line& line );

		glm::mat4 GetModelMatrix( glm::ivec2 offset, glm::ivec2 scale );

		glt::VAO				m_VAO;
		glt::VBO<glm::ivec2>	m_VBO;

		CStringDrawer			m_SDrawer;
	};
}