#pragma once
class CGame;
class CEntity;

class CComponent {
public:
	CComponent( CEntity* entity );
	virtual ~CComponent( );

	CEntity* const GetEntity( );
	CGame* const GetGame( );

private:
	CEntity* const	m_Entity;
};