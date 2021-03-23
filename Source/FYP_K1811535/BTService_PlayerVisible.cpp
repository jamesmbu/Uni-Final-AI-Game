// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerVisible.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_PlayerVisible::UBTService_PlayerVisible()
{
	NodeName = "Update Player Location if Seen";
}

void UBTService_PlayerVisible::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get Player Pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn || !OwnerComp.GetAIOwner())
	{
		return;
	}

	// Check if the AI has line of sight of player
	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation()
		);
	}
	
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

}
