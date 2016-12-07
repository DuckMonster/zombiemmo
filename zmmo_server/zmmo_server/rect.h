#pragma once
#include <types.h>

class rect {
public:
	static std::vector<glm::ivec2> Subtract( const rect& a, const rect& b );

public:
	glm::ivec2 m_Position;
	glm::ivec2 m_Size;

	rect( ) : m_Position( 0 ), m_Size( 0 ) { }
	rect( glm::ivec2 position ) : rect( position, glm::ivec2( 1 ) ) { }
	rect( glm::ivec2 position, glm::ivec2 size ) : m_Position( position ), m_Size( size ) { }

	bool Contains( glm::ivec2 v ) const {
		glm::ivec2 min = m_Position;
		glm::ivec2 max = m_Position + m_Size;

		return v.x >= min.x && v.x < max.x &&
			v.y >= min.y && v.y < max.y;
	}

	std::vector<glm::ivec2> Subtract( const rect& other ) { return std::move( Subtract( *this, other ) ); }
};
