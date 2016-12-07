#pragma once
#include <glm/vec2.hpp>

class ProgramInterface {
public:
	virtual void Load( ) { }
	virtual void Dispose( ) { }
	virtual void Update( float delta ) = 0;
};

class CFramework {
public:
	CFramework( );
	~CFramework( );

	void Run( ProgramInterface& program );
	void Exit( ) { m_Running = false; }

private:
	bool	m_Running;
};