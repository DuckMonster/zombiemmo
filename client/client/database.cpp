#include "stdafx.h"
#include "database.h"

#include <fstream>
#include "jsontools.h"
#include "tiletext.h"

MapDB::MapDB( std::string filename ) :
	m_Root( ) {

	m_Root << std::ifstream( filename );
}

json& MapDB::GetTile( unsigned char r, unsigned char g, unsigned char b ) {
	std::string s = ColorToString( r, g, b );
	json::iterator it = jsontools::FindKeyValue( m_Root.begin( ), m_Root.end( ), "color", s );

	return *it;
}

ActorDB::ActorDB( std::string filename ) {
	Parse( filename );
}

ActorDB::ActorData ActorDB::GetData( size_t index ) {
	return m_DataBase[index];
}

void ActorDB::Parse( std::string filename ) {
	json doc = json::parse( std::ifstream( "data/actor.json" ) );

	for (json::iterator i = doc["data"].begin( ); i != doc["data"].end( ); i++) {
		json& actor = *i;

		ActorData data;

		data.m_ID = actor["id"];
		data.m_TileIndex = GetTileChar( actor["char"].get<std::string>( )[0] );
		data.m_TileColor = StringToColor( actor["color"] );

		std::string name = actor["name"];
		std::string description = actor["description"];

		data.m_Name = name;
		data.m_Description = description;

		m_DataBase[data.m_ID] = data;
	}
}
