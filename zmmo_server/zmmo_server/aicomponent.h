#pragma once
#include "component.h"
#include "mob.h"
#include "timer.h"

class CAIComponent : public CComponent {
public:
	CAIComponent( CMob* mob );

	CActor* GetTarget( );
	virtual void Update( float delta );

private:
	struct Target {
	public:
		Target( ) : m_ID( 0 ), m_Active( false ) { }
		Target( CActor* e ) : m_ID( e != NULL ? e->GetID( ) : 0 ), m_Active( e != NULL ) { }

		entity_id GetID( ) { return m_ID; }
		operator bool( ) { return m_Active; }

	private:
		entity_id	m_ID;
		bool		m_Active;
	};

	void Move( );
	void MoveTowards( glm::ivec2 target );

	void LookForTarget( );
	void ReleaseTarget( );

	CMob* const		m_Mob;
	Target			m_Target;
	Timer			m_WalkTimer;
};