// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/CharacterBase.h"
#include "Perception/AIPerceptionTypes.h"

AMinionAIController::AMinionAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	AAIController::SetGenericTeamId(FGenericTeamId(1));
	ConfigureAIPerception();
	
}
void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehaviorTree)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		//ACharacterBase* Main = Cast<ACharacterBase>(PlayerPawn);
		
		RunBehaviorTree(AIBehaviorTree);
		
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		
	}
	
}

void AMinionAIController::ConfigureAIPerception()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AAIController::GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	AAIController::GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMinionAIController::OnPawnDetected);
	AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMinionAIController::OnTargetPerceptionUpdated);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}


void AMinionAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
		//GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeePlayer"), false);
	}
	
}

void AMinionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

/*FRotator AMinionAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}*/

void AMinionAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for(int i = 0; i < DetectedPawns.Num(); i++)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(DetectedPawns[i]);
		if (Main) // detected pawn is the player
		{
			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
			/*bIsPlayerDetected = true;
			GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeePlayer"), true);*/
		}
	}
	
	
}

void AMinionAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// continue from here :D
		
		UE_LOG(LogTemp, Warning, TEXT("Perception Changed "));

		if (Cast<ACharacterBase>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player "));
			bIsPlayerDetected = true;
			GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeePlayer"), true);
		}
		
	}
	else if (!Stimulus.WasSuccessfullySensed() && Cast<ACharacterBase>(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("no sense "));
		GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeePlayer"), false);
	}
}

void AMinionAIController::OnTargetDetected()
{
}






