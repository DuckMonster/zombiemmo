#pragma once
class CGame;

class CShadowController {
public:
	CShadowController( CGame* game );
	void Build( glm::ivec2& origin );

	bool IsVisible( const glm::ivec2& position );

	const std::vector<glm::ivec2>& GetVisible( ) { return m_Visible; }
	const std::vector<glm::ivec2>& GetHidden( ) { return m_Hidden; }

private:
	CGame* const				m_Game;

	glm::ivec2					m_LastOrigin;
	std::vector<glm::ivec2>		m_Visible;
	std::vector<glm::ivec2>		m_Hidden;
};