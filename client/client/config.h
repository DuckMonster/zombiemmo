#pragma once
#include <json.hpp>
//#include <sstream>
using json = nlohmann::json;

class Config {
public:
	static json root;

	static void Load( );
	template<typename T>
	static T Get( std::string query );
	static json Get( std::string query );
};

inline json Config::Get( std::string query ) {
	std::stringstream ss( query );
	std::string line;

	json j = root;

	while (!ss.eof( )) {
		getline( ss, line, '.' );
		j = j[line];
	}

	return j;
}

template<typename T>
inline T Config::Get( std::string query ) {
	return Get( query ).get<T>( );
}