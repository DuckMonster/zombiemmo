#include "stdafx.h"
#include "playercontroller.h"
#include "game.h"

CPlayerController::CPlayerController( CActor* actor, player_id playerID ) :
	CActorController( actor ), m_ID( playerID ) {
}
