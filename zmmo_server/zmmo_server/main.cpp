#include "stdafx.h"
#include "framework.h"
#include "program.h"

int main( ) {
	Config::Load( );

	CFramework framework;
	CProgram program( &framework );

	framework.Run( program );
}