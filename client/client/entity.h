#pragma once
class CGame;

class CEntity {
private:
	enum EVisibility {
		EKnown,
		ELast,
		ENone
	};

public:
	CEntity( CGame* game, size_t id );
	virtual ~CEntity( ) { };

	virtual void Update( float delta );
	virtual void Render( float delta );

	void Destroy( );

	glm::ivec2 GetPosition( ) { return m_Position; }
	virtual void SetPosition( glm::ivec2 position );
	void AddPosition( glm::ivec2 delta ) { SetPosition( m_Position + delta ); }

	int GetCollision( ) { return m_Collision; }
	void UpdateVisibility( );

	CGame* const GetGame( );
	const size_t GetID( ) { return m_ID; }

protected:
	void SetSprite( TileData data );
	CTileDrawer& GetDrawer( ) { return m_Drawer; }
	void SetCollision( int collision ) { m_Collision = collision; }

	glm::ivec2		m_Position;
	glm::ivec2		m_VisiblePosition;

private:
	const size_t	m_ID;

	CGame* const	m_Game;
	CTileDrawer		m_Drawer;

	int				m_Collision;
	EVisibility		m_Visibility;
};