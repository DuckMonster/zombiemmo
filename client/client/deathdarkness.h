#pragma once
class CDeathDarkness {
public:
	CDeathDarkness( glm::ivec2 size );

	void Render( glm::ivec2 origin, float scale );

private:
	CTileDrawer		m_Drawer;
};