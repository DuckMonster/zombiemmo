#pragma once
class CActor;
class CGame;

class CActorController {
public:
	CActorController( CActor* pawn );

	void Update( float delta );

	void Possess( CActor* pawn );
	void OnHit( );
	void OnDeath( );

	CActor* GetPawn( ) { return m_Pawn; }
	CGame* GetGame( );

private:
	CActor* const	m_Pawn;
};