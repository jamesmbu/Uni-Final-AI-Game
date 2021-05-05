// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_TrackLeader.generated.h"

/**
 * 
 */
UCLASS()
class FYP_K1811535_API UMyBTService_TrackLeader : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMyBTService_TrackLeader();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
