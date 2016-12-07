#include "stdafx.h"
#include "cone.h"

cone::cone( glm::ivec2 origin, float direction, float angle, int radius ) :
	m_Origin( origin ),
	m_Direction( direction ),
	m_Angle( angle ),
	m_Radius( radius ) {

	Build( );
}

void cone::Build( ) {
	glm::vec2 o = m_Origin;
	glm::vec2 dir = math::angToVecD( m_Direction );

	int radiusSqrd = m_Radius * m_Radius;

	for (int x = -m_Radius; x <= m_Radius; x++) {
		for (int y = -m_Radius; y <= m_Radius; y++) {
			if (x == 0 && y == 0) {
				m_Points.push_back( m_Origin + glm::ivec2( x, y ) );
				continue;
			}

			if (x * x + y * y > radiusSqrd)
				continue;

			float a = math::vecToAngD( glm::vec2( x, y ) );
			if (glm::abs( math::deltaAngle( m_Direction, a ) ) > m_Angle)
				continue;

			m_Points.push_back( m_Origin + glm::ivec2( x, y ) );
		}
	}
}
