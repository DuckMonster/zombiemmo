#include "stdafx.h"
#include "database.h"

#include <fstream>

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

		std::string name = actor["name"];
		std::string description = actor["description"];

		data.m_Name = name;
		data.m_Description = description;

		m_DataBase[data.m_ID] = data;
	}
}
