#include "stdafx.h"
#include "inputcontroller.h"

CInputController::CInputController( ) :
	m_Receiver( NULL ), m_TickValue( 0.f ), m_Input( ) {
}

void CInputController::Update( float delta ) {
	if (!m_Receiver)
		return;

	m_TickValue = glm::clamp( m_TickValue + delta * m_Receiver->GetInputTickFrequency( ), 0.f, 1.f );

	if (m_TickValue >= 1.f && !m_Input.empty( )) {

		if (m_Receiver->GetInputTopOnly( ))
			m_Receiver->InputTick( m_Input.back( ) );
		else
			for (sf::Keyboard::Key& k : m_Input)
				m_Receiver->InputTick( k );
		m_TickValue = 0.f;
	}
}

void CInputController::SetReceiver( IInputReceiver* receiver ) {
	m_Receiver = receiver;
}

void CInputController::AddInput( sf::Keyboard::Key k ) {
	auto i = std::find( m_Input.begin( ), m_Input.end( ), k );
	if (i != m_Input.end( ))
		return;

	m_Input.push_back( k );
}

void CInputController::ReleaseInput( sf::Keyboard::Key k ) {
	auto i = std::find( m_Input.begin( ), m_Input.end( ), k );
	if (i != m_Input.end( )) {
		m_Input.erase( i );

		if (m_Receiver)
			m_Receiver->InputReleased( k );
	}
}
