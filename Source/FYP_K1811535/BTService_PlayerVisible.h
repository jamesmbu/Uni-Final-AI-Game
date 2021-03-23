// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerVisible.generated.h"

/**
 * 
 */
UCLASS()
class FYP_K1811535_API UBTService_PlayerVisible : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_PlayerVisible();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
