#include "stdafx.h"
#include "line.h"

line::line( glm::ivec2 a, glm::ivec2 b ) : m_First( a ), m_Second( b ) {
	Build( );
}

bool line::Contains( const glm::ivec2& v ) {
	return std::find( m_Points.begin( ), m_Points.end( ), v ) != m_Points.end( );
}

void line::Build( ) {
	glm::ivec2 dif = m_Second - m_First;
	if (dif == glm::ivec2( 0 )) {
		m_Points.push_back( m_First );
		return;
	}

	int distance = glm::max( glm::abs( dif.x ), glm::abs( dif.y ) );

	glm::vec2 begin( m_First );
	glm::vec2 len( dif );

	len /= distance;

	for (size_t i = 0; i <= distance; i++) {
		glm::ivec2 point = glm::round( begin + len * (float)(i) );
		m_Points.push_back( point );
	}
}