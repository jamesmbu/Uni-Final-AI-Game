// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Melee.h"
#include "AIController.h"
#include "FYP_K1811535/Characters/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_Melee::UMyBTTask_Melee()
{
	NodeName = TEXT("Melee");
}

EBTNodeResult::Type UMyBTTask_Melee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAttacking", true);
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	// Get Pawn reference to this enemy character
	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Get Pawn reference to the player
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Initiate melee attack from the enemy character to the player
	Character->Melee(PlayerPawn);

	return EBTNodeResult::Succeeded;
}
