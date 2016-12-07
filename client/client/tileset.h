#pragma once
class CTileset {
public:
	static CTileset* Active;

public:
	glm::ivec2		m_Size;
	glt::Texture	m_Texture;

	CTileset( ) :
		m_Size( 0 ), m_Texture( ) {
	}

	void Init( glm::ivec2 size, std::string& filename ) {
		m_Size = size;
		m_Texture.loadFile( filename.c_str( ) );
	}
};