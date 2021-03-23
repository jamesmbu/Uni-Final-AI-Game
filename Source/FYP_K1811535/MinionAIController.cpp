// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CharacterBase.h"

void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehaviorTree)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		ACharacterBase* Main = Cast<ACharacterBase>(PlayerPawn);
		
		RunBehaviorTree(AIBehaviorTree);
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		
	}
	
}
void AMinionAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
}



