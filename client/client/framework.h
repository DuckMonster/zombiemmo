#pragma once
class ProgramInterface {
public:
	virtual void Load( ) { }
	virtual void Dispose( ) { }
	virtual void Update( float delta ) = 0;
	virtual void Render( float delta ) = 0;
	virtual void KeyEvent( sf::Keyboard::Key key, int state ) = 0;
};

class CFramework {
public:
	CFramework( const size_t width, const size_t height, char* title );
	~CFramework( );

	void Run( ProgramInterface& program );
	glm::ivec2 WindowSize( ) { return{ m_Window.getSize( ).x, m_Window.getSize( ).y }; }

	void Exit( ) { m_ExitFlag = true; }

private:
	bool			m_ExitFlag;
	sf::Window		m_Window;
};