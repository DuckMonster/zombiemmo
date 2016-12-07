#pragma once
#include "entity.h"

class CCameraController {
public:
	CCameraController( int zoom );

	glm::mat4 GameMatrix( );
	glm::mat4 UIMatrix( );

	CEntity*	m_Target;
	int			m_Zoom;
};