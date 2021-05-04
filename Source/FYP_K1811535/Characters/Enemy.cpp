// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FYP_K1811535/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "FYP_K1811535/MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bOverlappingCombatSphere = false;
	
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(GetRootComponent());
	DetectionSphere->InitSphereRadius(600.f);
	
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(150.f);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));
	
	AttackLightTime = 1.0f;
	AttackHeavyTime = 5.0f;
	Damage = 25.f;
	HeavyAttackDamageMultiplier = 1.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDelay = 0.6f;
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive())
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attack);
		}
		if (!bAttacking)
		{
			int _index = FMath::RandRange(0, 1);
			
			AttackTime = AttackSpeeds[_index];
			if (_index == 0)
			{
				Damage = DamageInitial * HeavyAttackDamageMultiplier;
			}
			else if (_index == 1)
			{
				Damage = DamageInitial;
			}
			float AnimationLength = CombatMontage->GetSectionLength(CombatMontage->GetSectionIndex(FName("Attack")));
			float scaleToDesiredAttackTime =  AnimationLength/ AttackTime;
			
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, scaleToDesiredAttackTime);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}
		}
	}
}

void AEnemy::Melee(AActor* OtherActor)
{
	HasBT = true;
	if (Alive())
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		{
			if (Main)
			{
				Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(true);
				if (Main->MainPlayerController)
				{
					Main->MainPlayerController->DisplayEnemyHealthBar();
				}
				CombatTarget = Main;
				Attack();

			}
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	
	if (bOverlappingCombatSphere && !HasBT)
	{
		//float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
	else if (!bOverlappingCombatSphere && !HasBT)
	{
		if (CombatTarget)
		{
			MoveToTarget(CombatTarget);
			GetWorldTimerManager().ClearTimer(AttackTimer);
			CombatTarget = nullptr;
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	HealthComponent->DecrementHealth(DamageAmount);
	if (HealthComponent->ActiveHealth <= 0)
	{
		Die();
	}
	return DamageAmount;
}

void AEnemy::Die()
{
	DetachFromControllerPendingDestroy();
	/* Play the death animation */
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection("Death");
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

	/* Remove any associated collision  */
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	//GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Vanish, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Vanish()
{
	Destroy();
}

// meant to be called with a timer
void AEnemy::DelayedMovement(ACharacterBase* Target) 
{
	MoveToTarget(Target);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectionSphereOnOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectionSphereOnOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatObjectOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatObjectOnOverlapEnd);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap);

	DamageInitial = Damage;
	AttackSpeeds = { AttackHeavyTime, AttackLightTime };
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DetectionSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::DetectionSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		{
			if (Main)
			{
				if (Main->CombatTarget == this)
				{
					Main->SetCombatTarget(nullptr);
				}
				Main->SetHasCombatTarget(false);
				if (Main->MainPlayerController)
				{
					Main->MainPlayerController->RemoveEnemyHealthBar();
				}
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive() && !HasBT)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		{
			if (Main)
			{
				Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(true);
				if (Main->MainPlayerController)
				{
					Main->MainPlayerController->DisplayEnemyHealthBar();
				}
				CombatTarget = Main;
				bOverlappingCombatSphere = true;
				//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attack);
				//float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, 0.8);
				
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		{
			if (Main)
			{
				bOverlappingCombatSphere = false;
				/*if (EnemyMovementStatus == EEnemyMovementStatus::EMS_Attack) // so movement is after the animation ends
				{
					MoveToTarget(Main);
					CombatTarget = nullptr;
				}*/
				if (!bAttacking)
				{
					GetWorldTimerManager().ClearTimer(AttackTimer);
					
					MoveToTarget(Main);
				}
			}
		}
	}
}

void AEnemy::CombatObjectOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
		ACharacterBase* Main = Cast<ACharacterBase>(OtherActor);
		if (Main)
		{
			if (DamageTypeClass)
			{
				//UE_LOG(LogTemp, Warning, TEXT("no"));
				UGameplayStatics::ApplyDamage(
					Main, Damage, AIController,
					this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatObjectOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::MoveToTarget(ACharacterBase* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Aarrr"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(100.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

