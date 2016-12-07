#include "stdafx.h"
#include "framework.h"

using namespace std;

CFramework::CFramework( ) : m_Running( true ) {
}

CFramework::~CFramework( ) {
}

void CFramework::Run( ProgramInterface & program ) {
	program.Load( );

	double fpsTotal = 0.f;
	float fpsMax = 0.f;
	float fpsMin = 50000.f;
	int frameCount = 0;

	while (m_Running) {
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

		// Update and render
		program.Update( delta );

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	program.Dispose( );
}