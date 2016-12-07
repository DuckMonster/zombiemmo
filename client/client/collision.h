#pragma once

namespace collision {
	enum Category {
		Map = 1 << 0,
		Entity = 1 << 1
	};

	enum Type {
		Solid = 1 << 0,
		Visibility = 1 << 1,
		Pickup = 1 << 2
	};

	template<typename T>
	struct HitResult {
	public:
		bool				m_Hit;
		glm::ivec2			m_Point;
		std::vector<T*>		m_HitList;

		HitResult( ) : m_Hit( 0 ), m_Point( 0 ), m_HitList( ) { }

		operator bool( ) { return m_Hit; };
	};
}

template<typename T>
bool QueryCollision( CGame* const game, glm::ivec2 position, collision::HitResult<T>* outResult = NULL, unsigned int typeMask = 0xFFFFFFFF, unsigned int collisionMask = collision::Solid ) {
	bool result = false;

	CMap* map = game->GetMap( );

	if ((typeMask & collision::Map) != 0)
		if ((map->GetTile( position ).m_Collision & collisionMask) != 0)
			result = true;

	if ((typeMask & collision::Entity) != 0) {
		const std::vector<entity_id>& entityList = game->AllEntites( );

		for (size_t i = 0; i < entityList.size( ); i++) {
			T* e = game->GetEntity<T>( entityList[i] );
			if (e == NULL)
				continue;

			if ((e->GetCollision( ) & collisionMask) != 0 && e->GetPosition( ) == position) {
				if (outResult)
					outResult->m_HitList.push_back( e );

				result = true;
			}
		}
	}

	if (outResult) {
		outResult->m_Point = position;
		if (result)
			outResult->m_Hit = true;
	}

	return result;
}

template<typename T>
bool QueryLineCollision( CGame* game, const line& line, collision::HitResult<T>* outResult = NULL, unsigned int typeMask = 0xFFFFFFFF, unsigned int collisionMask = collision::Solid ) {
	const std::vector<glm::ivec2>& points = line.GetPoints( );

	for (size_t i = 1; i < points.size( ) - 1; i++) {
		if (QueryCollision( game, points[i], outResult, typeMask, collisionMask ))
			return true;
	}

	if (outResult) {
		outResult->m_Point = line.Second( );
	}
	return false;
}