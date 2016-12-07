#include "stdafx.h"

#include "tiletext.h"

namespace {
	static const glm::ivec2 BIG_CHARS( 0, 1 );
	static const glm::ivec2 SMALL_CHARS( 0, 2 );
	static const glm::ivec2 SPEC_01( 0, 3 );
	static const glm::ivec2 SPEC_02( 0, 4 );
	static const glm::ivec2 SPEC_03( 0, 5 );
	static const glm::ivec2 SPEC_04( 0, 6 );

	static const glm::vec4 DEFAULT_COLOR = glm::vec4( 1.f );
	static const std::string COLOR_FORMAT = "~C[";

	static const int ABBA( 25 );
}

glm::ivec2 GetTileChar( char c ) {
	if (c >= 'A' && c <= 'Z')
		return glm::ivec2( BIG_CHARS + glm::ivec2( c - 'A', 0 ) );

	else if (c >= 'a' && c <= 'z')
		return glm::ivec2( SMALL_CHARS + glm::ivec2( c - 'a', 0 ) );

	else if (c >= ' ' && c <= '9')
		return glm::ivec2( SPEC_01 + glm::ivec2( c - ' ', 0 ) );

	else if (c >= ':' && c <= '@')
		return glm::ivec2( SPEC_02 + glm::ivec2( c - ':', 0 ) );

	else if (c >= '[' && c <= '`')
		return glm::ivec2( SPEC_03 + glm::ivec2( c - '[', 0 ) );

	else if (c >= '{' && c <= '~')
		return glm::ivec2( SPEC_04 + glm::ivec2( c - '{', 0 ) );

	else if (c == '\n')
		return GetTileChar( ' ' );

	else
		return GetTileChar( '?' );
}

std::vector<TileData> GetTileString( std::string str, std::vector<glm::vec4> colors ) {
	std::vector<TileData> data;
	data.reserve( str.length( ) );

	glm::ivec2 position( 0.f );
	glm::vec4 color = DEFAULT_COLOR;

	for (size_t i = 0; i < str.length( ); i++) {
		char c = str[i];

		if (str.compare( i, COLOR_FORMAT.length( ), COLOR_FORMAT ) == 0) {
			i += COLOR_FORMAT.length( );

			int colorIndex = std::stoi( str.substr( i, 1 ) );
			if (colorIndex >= 0 && colorIndex < colors.size( ))
				color = colors[colorIndex];

			continue;
		}

		data.push_back( TileData( GetTileChar( c ), position, color ) );

		if (c == '\n') {
			position = glm::ivec2( 0.f, position.y + 1 );
			continue;
		}

		position.x++;
	}

	return data;
}

std::vector<TileData> GetTileString( std::string str ) { return GetTileString( str, { } ); }