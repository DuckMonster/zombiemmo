#include "stdafx.h"
#include "config.h"

#include <fstream>

json Config::root;

void Config::Load( ) {
	root["system"] << std::ifstream( "data/system.json" );
	root["gfx"] << std::ifstream( "data/gfx.json" );
}