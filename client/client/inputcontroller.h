#pragma once
#include <vector>
class IInputReceiver {
public:
	virtual void InputReleased( sf::Keyboard::Key k ) = 0;
	virtual void InputTick( sf::Keyboard::Key k ) = 0;

	virtual float GetInputTickFrequency( ) = 0;
	virtual bool GetInputTopOnly( ) = 0;
};

class CInputController {
public:
	CInputController( );

	void Update( float delta );
	void SetReceiver( IInputReceiver* receiver );

	void AddInput( sf::Keyboard::Key k );
	void ReleaseInput( sf::Keyboard::Key k );

private:
	IInputReceiver*					m_Receiver;
	float							m_TickValue;

	std::list<sf::Keyboard::Key>	m_Input;
};