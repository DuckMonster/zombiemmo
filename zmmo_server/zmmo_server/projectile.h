#pragma once
class CActor;

class CProjectile {
public:
	CProjectile( CActor* host, glm::ivec2 origin, glm::ivec2 target );
	void Update( float delta );

	bool IsDestroyed( ) { return m_Destroyed; }

	void SetID( size_t id ) { m_ID = id; }
	size_t GetID( ) { return m_ID; }

	glm::ivec2 GetPosition( );

private:
	float GetPercent( ) { return glm::clamp( (m_Velocity * m_Time) / m_Distance, 0.f, 1.f ); }
	int GetLineIndex( ) { return (int)((float)(m_Line.GetPoints( ).size( ) - 1) * GetPercent( )); }

	void HitPosition( int index );

	void Destroy( ) { m_Destroyed = true; }

	size_t			m_ID;

	CActor*			m_Host;

	line			m_Line;
	bool			m_Destroyed;

	float			m_Time;
	float			m_Distance;
	float			m_Velocity;
};