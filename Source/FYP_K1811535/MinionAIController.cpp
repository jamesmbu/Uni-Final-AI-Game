// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"
#include "Kismet/GameplayStatics.h"

void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	SetFocus(PlayerPawn);
}
