#include "FYP_K1811535GameModeBase.h"

void AFYP_K1811535GameModeBase::BeginPlay()
{
	// a good place to retrieve references to conditions in the game which go towards winning or losing/ score
}

void AFYP_K1811535GameModeBase::HandleGameStart()
{
	// can have a countdown, checking number of enemies
}

void AFYP_K1811535GameModeBase::HandleGameOver(bool PlayerWon)
{
	// check if the conditions have been met
}

void AFYP_K1811535GameModeBase::DeathHandle(AActor* DeadActor)
{
	// check who has died (player/enemy/boss)
}

