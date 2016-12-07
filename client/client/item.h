#pragma once
struct Item {
	std::string		m_Name;
	std::string		m_Description;
	glm::ivec2		m_Icon;

	Item( ) :
		m_Name( "[NONE]" ), m_Description( "[NONE]" ), m_Icon( 0 ) { }
	Item( std::string name, std::string description, glm::ivec2 icon):
		m_Name(name), m_Description( description), m_Icon(icon) { }
};