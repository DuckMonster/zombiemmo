#include "stdafx.h"
#include "rect.h"

std::vector<glm::ivec2> rect::Subtract( const rect& a, const rect& b ) {
	std::vector<glm::ivec2> result;

	glm::ivec2 index( 0 );

	for (; index.y < a.m_Size.y; ++index.y)
		for (index.x = 0; index.x < a.m_Size.x; ++index.x) {
			if (!b.Contains( a.m_Position + index ))
				result.push_back( a.m_Position + index );
		}

	return std::move( result );
}