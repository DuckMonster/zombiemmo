#include "stdafx.h"

#include "framework.h"
#include "program.h"
#include "config.h"
#include "database.h"

int main( ) {
	Config::Load( );

	CFramework framework(
		Config::Get<int>( "gfx.tileset.width" ) * Config::Get<int>( "system.console.width" ),
		Config::Get<int>( "gfx.tileset.height" ) * Config::Get<int>("system.console.height"), "ZombieMMO Client" );
	//CFramework framework( 1024, 768, "ZombieMMO Client" );
	CProgram program( &framework );

	framework.Run( program );
}