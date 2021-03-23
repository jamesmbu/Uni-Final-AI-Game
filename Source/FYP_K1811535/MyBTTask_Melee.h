// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Melee.generated.h"

/**
 * 
 */
UCLASS()
class FYP_K1811535_API UMyBTTask_Melee : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMyBTTask_Melee();

	float AttackInterval;
	FTimerHandle TimerHandle;

private:
	void Melee();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
