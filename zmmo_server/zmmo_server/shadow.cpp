#include "stdafx.h"
#include "shadow.h"

using namespace glm;

namespace {
	const size_t RANGE = 10;

	struct Shadow {
		float m_Min;
		float m_Max;

		Shadow( glm::ivec2 tile, float scale ) :
			m_Min( (float)tile.x / (tile.y + scale) ),
			m_Max( (float)(tile.x + scale) / tile.y ) {
		}

		Shadow( glm::ivec2 tile ) : Shadow( tile, 1.f ) { }

		Shadow( float min, float max ) :
			m_Min( min ), m_Max( max ) { }

		bool Contains( Shadow& other ) {
			return m_Min <= other.m_Min && m_Max >= other.m_Max;
		}

		bool Overlaps( Shadow& other ) {
			return m_Max >= other.m_Min && other.m_Max >= m_Min;
		}

		void Merge( Shadow& other ) {
			m_Min = glm::min( m_Min, other.m_Min );
			m_Max = glm::max( m_Max, other.m_Max );
		}

		std::string ToString( ) {
			return std::string( "{ " ) + std::to_string( m_Min ) + ", " + std::to_string( m_Max ) + " }\n";
		}
	};

	class ShadowCollection {
	public:
		void Add( Shadow& other ) {
			// Find insertion index ( sort from smallest to biggest )
			size_t index = 0;
			for (; index < m_Shadows.size( ); index++) {
				if (m_Shadows[index].m_Min >= other.m_Min)
					break;
			}

			bool prevOverlap = (index > 0 && m_Shadows[index - 1].Overlaps( other ));
			bool nextOverlap = (index < m_Shadows.size( ) && m_Shadows[index].Overlaps( other ));

			if (prevOverlap) {
				// Overlaps prev AND next, grow previous and remove next
				if (nextOverlap) {
					m_Shadows[index - 1].Merge( m_Shadows[index] );
					m_Shadows.erase( m_Shadows.begin( ) + index );
				}

				// Overlaps previous, merge
				else
					m_Shadows[index - 1].Merge( other );
			}

			// Overlaps next, merge
			else if (nextOverlap) {
				m_Shadows[index].Merge( other );
			}

			// No overlap
			else {
				m_Shadows.insert( m_Shadows.begin( ) + index, other );
			}
		}

		bool Contains( Shadow& other ) {
			for (Shadow& s : m_Shadows)
				if (s.Contains( other ))
					return true;

			return false;
		}

		std::vector<Shadow> m_Shadows;
	private:
	};
}

glm::ivec2 Octant( int row, int col, int oct ) {
	switch (oct) {
		case 0: return glm::ivec2( col, -row );
		case 1: return glm::ivec2( row, -col );
		case 2: return glm::ivec2( row, col );
		case 3: return glm::ivec2( col, row );
		case 4: return glm::ivec2( -col, row );
		case 5: return glm::ivec2( -row, col );
		case 6: return glm::ivec2( -row, -col );
		case 7: return glm::ivec2( -col, -row );
	}
}

void shadow::GetShadows( glm::ivec2 origin, CGame & game, std::vector<glm::ivec2>& hidden, std::vector<glm::ivec2>& visible ) {
	hidden.clear( );
	visible.clear( );

	hidden.reserve( RANGE * RANGE / 2 * 8 );
	visible.reserve( RANGE * RANGE / 2 * 8 );
	visible.push_back( origin );

	int rangeSqrd = RANGE * RANGE;

	// 8 Octants
	for (int i=0; i < 8; i++) {

		ShadowCollection shadowCollection;
		glm::ivec2 coord = glm::ivec2( 0, 1 );

		for (; coord.y < RANGE; coord.y++) {
			for (coord.x = 0; coord.x <= coord.y; coord.x++) {

				// Radii is bigger than range
				if (coord.x * coord.x + coord.y * coord.y > rangeSqrd)
					continue;

				glm::ivec2 world = Octant( coord.y, coord.x, i );

				bool collision = true;
				//bool collision = QueryCollision( &game,  )
				//bool collision = game.GetCollision( origin + world, NULL, collision::Map, collision::Visibility );
				Shadow tileShadow( coord );

				// Hidden
				if (shadowCollection.Contains( tileShadow )) {

					// If this octant is a multiple of two, don't count overlapping tiles
					if (i % 2 == 0 &&
						 (coord.x == 0 || coord.x == coord.y))
						continue;

					hidden.push_back( origin + world );
					continue;
				}

				// Visible
				else {
					visible.push_back( origin + world );

					// This is a shadow-caster
					if (collision)
						shadowCollection.Add( tileShadow );
				}

			}
		}
	}
}