// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_TrackLeader.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_TrackLeader::UMyBTService_TrackLeader()
{
	NodeName = "Track Leader";
	
}

void UMyBTService_TrackLeader::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UObject* dff = OwnerComp.GetBlackboardComponent()->GetValueAsObject(
		"NoisyFriend"
	);
	AActor* Actor = Cast<AActor>(dff);
	if (Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor"));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			GetSelectedBlackboardKey(), 
			Actor->GetActorLocation());
	}
	
}
