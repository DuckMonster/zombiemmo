#pragma once
#include <json.hpp>
using json = nlohmann::json;

class MapDB {
public:
	MapDB( std::string filename );
	json& GetTile( unsigned char r, unsigned char g, unsigned char b );

private:
	json	m_Root;
};

class ActorDB {
public:
	struct ActorData {
		size_t		m_ID;
		glm::ivec2	m_TileIndex;
		color		m_TileColor;

		std::string	m_Name;
		std::string	m_Description;

		ActorData( ) : m_ID( -1 ), m_TileIndex( 0 ), m_TileColor( 0 ), m_Name( "" ), m_Description( "" ) { }
	};

public:
	ActorDB( std::string filename );

	ActorData GetData( size_t index );

private:
	void Parse( std::string filename );
	std::map<size_t, ActorData>	m_DataBase;
};