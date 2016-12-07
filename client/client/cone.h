#pragma once
class cone {
public:
	cone( glm::ivec2 origin, float direction, float angle, int radius );

	const std::vector<glm::ivec2>& GetPoints( ) { return m_Points; }

private:
	void Build( );

	glm::ivec2				m_Origin;
	float					m_Direction;
	float					m_Angle;
	int						m_Radius;

	std::vector<glm::ivec2>	m_Points;
};