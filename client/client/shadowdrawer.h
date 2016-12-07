#pragma once
class CShadowDrawer {
public:
	static glt::Shader& GetShader( );
	
private:
	static glt::Shader& GetShadowShader( );

public:
	CShadowDrawer( );

	void Init( );
	void Build( const std::vector<glm::ivec2>& hidden );
	void Render( glt::Texture& frame );

private:
	VAO					m_vao;
	VBO<glm::ivec2>		m_vbo;

	VAO					m_shadowVAO;
	VBO<glm::vec2>		m_shadowVBO;

	size_t				m_vertexCount;
};