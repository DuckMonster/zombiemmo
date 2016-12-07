#include "stdafx.h"

#include "framework.h"

using namespace std;

CFramework::CFramework( const size_t width, const size_t height, char* title ) :
	m_ExitFlag( false ), m_Window( ) {
	sf::ContextSettings settings;

	settings.depthBits = 24;
	settings.stencilBits = 8;

	m_Window.create( sf::VideoMode( width, height ), title, sf::Style::Default, settings );
	//m_Window.setFramerateLimit( 500 );
	m_Window.setKeyRepeatEnabled( false );

	glt::gltInit( );
	glt::debug::logLevel = 2;

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

CFramework::~CFramework( ) {
}

void CFramework::Run( ProgramInterface & program ) {
	program.Load( );

	double fpsTotal = 0.f;
	float fpsMax = 0.f;
	float fpsMin = 50000.f;
	int frameCount = 0;


	while (m_Window.isOpen( )) {
		float delta = glt::Time::delta( );

		fpsTotal += delta;
		frameCount++;

		if (delta > fpsMax)
			fpsMax = delta;
		if (delta < fpsMin)
			fpsMin = delta;

		if (fpsTotal > 1.f) {
			std::cout << (fpsTotal / frameCount) * 1000.f << " ms ( MAX " << fpsMax * 1000.f << " ms, MIN " << fpsMin * 1000.f << " ms)\n";

			fpsTotal = 0.f;
			fpsMax = 0.f;
			fpsMin = 50000.f;
			frameCount = 0;
		}

		// Poll events
		sf::Event e;
		while (m_Window.pollEvent( e )) {
			switch (e.type) {
				case sf::Event::Closed:
					m_Window.close( );
					break;

				case sf::Event::KeyPressed:
					program.KeyEvent( e.key.code, 1 );
					break;

				case sf::Event::KeyReleased:
					program.KeyEvent( e.key.code, 0 );
					break;
			}
		}

		// Update and render
		program.Update( delta );
		program.Render( delta );

		//

		m_Window.display( );

		if (m_ExitFlag)
			m_Window.close( );

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	program.Dispose( );
}