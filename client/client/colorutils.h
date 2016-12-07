#pragma once
typedef glm::vec4 color;
inline color HexToColor( unsigned int hex ) {
	return color(
		((hex >> 16) & 0xFF) / 255.f,
		((hex >> 8) & 0xFF) / 255.f,
		(hex & 0xFF) / 255.f,
		1.f
		);
}

inline std::string ColorToString( unsigned char r, unsigned char g, unsigned char b ) {
	std::stringstream str;
	str << std::hex << (int)r << (int)g << (int)b;
	return str.str( );
}

inline color StringToColor( std::string str ) {
	int val;

	std::stringstream ss;
	ss << std::hex << str;
	ss >> val;

	return HexToColor( val );
}

struct ColorGradient {
public:
	color m_Left;
	color m_Right;

	ColorGradient( color left, color right ) :
		m_Left( left ), m_Right( right ) { }

	color Lerp( float v ) const {
		glm::clamp( v, 0.f, 1.f );
		return m_Left + (m_Right - m_Left) * v;
	}
};