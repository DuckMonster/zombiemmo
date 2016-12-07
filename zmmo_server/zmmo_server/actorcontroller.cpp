#include "stdafx.h"
#include "actorcontroller.h"

#include "actor.h"
#include "game.h"

CActorController::CActorController( CActor * pawn ) : 
	m_Pawn( pawn ) {
}

void CActorController::Update( float delta ) {
}

void CActorController::Possess( CActor * pawn ) {
}

void CActorController::OnHit( ) {
}

void CActorController::OnDeath( ) {
}

CGame* CActorController::GetGame( ) {
	return m_Pawn->GetGame( );
}
