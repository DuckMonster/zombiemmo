#pragma once
#include <json.hpp>
using json = nlohmann::json;

class ActorDB {
public:
	struct ActorData {
		size_t		m_ID;

		std::string	m_Name;
		std::string	m_Description;

		ActorData( ) : m_ID( -1 ), m_Name( "" ), m_Description( "" ) { }
	};

public:
	ActorDB( std::string filename );

	ActorData GetData( size_t index );

private:
	void Parse( std::string filename );
	std::map<size_t, ActorData>	m_DataBase;
};