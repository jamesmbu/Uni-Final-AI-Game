// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NameTypes.h"
#include "Perception/AIPerceptionTypes.h"
#include "MinionAIController.generated.h"

struct FAIStimulus;
/**
 * 
 */
class UBehaviorTree;
class UAISenseConfig_Sight;
UCLASS()
class FYP_K1811535_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMinionAIController();
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override; // When the character plugs into the controller

	//virtual FRotator GetControlRotation() const override; // For rotation of perception

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns); // When any pawn is detected by perception

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnTargetDetected();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 500.0f; // Distance-based visibility
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f; // How long an object in sight is remembered for
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.0f; // Distance in which AI loses sight of a previously visible object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehaviorTree;

	void ConfigureAIPerception();
};
