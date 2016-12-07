#pragma once
class line {
public:
	line( glm::ivec2 a, glm::ivec2 b );

	const std::vector<glm::ivec2>& GetPoints( ) const { return m_Points; }
	bool Contains( const glm::ivec2& v ) const;

	const glm::ivec2 First( ) const { return m_First; }
	const glm::ivec2 Second( ) const { return m_Second; }

private:
	void Build( );

	glm::ivec2 m_First, m_Second;
	std::vector<glm::ivec2> m_Points;
};