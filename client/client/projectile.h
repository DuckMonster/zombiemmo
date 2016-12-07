#pragma once
class CActor;

class CProjectile {
public:
	CProjectile( CActor* host, glm::ivec2 origin, glm::ivec2 target );
	void Update( float delta );
	void Render( float delta );

	bool IsDestroyed( ) { return m_Destroyed; }
	void Hit( size_t index );

	glm::ivec2 GetPosition( );

private:
	float GetPercent( ) { return glm::clamp( (m_Velocity * m_Time) / m_Distance, 0.f, 1.f ); }
	int GetIndex( ) { return (int)((float)(m_Line.GetPoints( ).size( ) - 1) * GetPercent( )); }

	void HitPosition( int index );

	void Destroy( ) { m_Destroyed = true; }
	void UpdateTiles( float delta );

	CActor*		m_Host;

	line		m_Line;
	bool		m_Destroyed;

	float		m_Time;
	float		m_Distance;
	float		m_Velocity;

	CTileDrawer	m_Drawer;
};