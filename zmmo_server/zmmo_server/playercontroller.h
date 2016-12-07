#pragma once
#include "actor.h"
#include "actorcontroller.h"

class CGame;

class CPlayerController : CActorController {
public:
	CPlayerController( CActor* actor, player_id playerID );

	size_t GetPlayerID( ) { return m_ID; }

private:
	const size_t		m_ID;
};