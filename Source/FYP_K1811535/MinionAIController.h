// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MinionAIController.generated.h"

struct FAIStimulus;
class UBehaviorTree;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
UCLASS()
class FYP_K1811535_API AMinionAIController : public AAIController
{
	GENERATED_BODY()
public:
	// Constructor
	AMinionAIController();

	/* AI Controller Overrides */
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override; // When the character plugs into the controller
	/* <end> */

	
	//virtual FRotator GetControlRotation() const override; // For rotation of perception

	
	/* AI Perception: Dynamic Delegates */
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns); // When any pawn is detected by perception

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnTargetDetected();
	/* <end> */

	
	/* AI Perception: Properties */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 800.0f; // Distance-based visibility
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f; // How long an object in sight is remembered for
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius * 1.1f; // Distance in which AI loses sight of a previously visible object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;
	/* <end> */

	/* Custom properties */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;
	/* <end> */

	/* Custom functions */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAgroSound(); // play a sound (configurable in the blueprint)
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPatrolBehaviour(); // patrolling is a behaviour I have implemented by blueprint using blueprint elements which cannot be referenced in C++
	/* <end> */

	
protected:
	
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehaviorTree;

	void ConfigureAIPerception();
};
