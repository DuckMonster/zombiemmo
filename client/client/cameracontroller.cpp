#include "stdafx.h"
#include "cameracontroller.h"

CCameraController::CCameraController( int zoom ) :
	m_Target( NULL ), m_Zoom( zoom ) {
}

glm::mat4 CCameraController::GameMatrix( ) {
	glm::mat4 mat( 1.f );

	if (m_Target)
		mat[3] = glm::vec4( -m_Target->GetPosition( ), 0.f, 1.f );

	return UIMatrix( ) * mat;
}

glm::mat4 CCameraController::UIMatrix( ) {
	glm::mat4 mat( 1.f );

	//mat[0][0] = m_Zoom;
	//mat[1][1] = m_Zoom;

	return mat;
}