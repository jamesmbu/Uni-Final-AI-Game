// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "FYP_K1811535/Modes/FYP_K1811535GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CharacterBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	
	/* HEALTH */
	MaxHealth = 100.f;
	ActiveHealth = MaxHealth;
	DamageCushioning = 0.f;
	HealthRecoveryRate = 2.f;

	/* STAMINA */
	MaxStamina = 100.f;
	ActiveStamina = MaxStamina;
	StaminaStatus = EStaminaStatus::ESS_Normal;
	StaminaDrainRate = 25.f;
	StaminaRecoveryRate = 15.f;
	MinSprintStamina = 25.f;
}

void UHealthComponent::DecrementHealth(float Amount)
{
	if (ActiveHealth - Amount <= 0.f)
	{
		ActiveHealth = 0.f;
		HandleDeath();
	}
		
	else
		ActiveHealth -= Amount;
}

void UHealthComponent::HandleDeath()
{
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	/* GAME-MODE REFERENCE*/
	GameModeReference = Cast<AFYP_K1811535GameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); // find the currently active game mode
	/* DELEGATE: RECEIVING DAMAGE */
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage); // when the owning actor calls OnTakeAnyDamage, the dynamic delegate will be called from this class using TakeDamage

	/* DELEGATE: SPRINT STATUS */
	owningCharacter = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (owningCharacter) // if owner is the character
	{
		owningCharacter->bPlayerSprinting.AddDynamic(this, &UHealthComponent::OnSprintStatusChange);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount == 0)
	{
		return;
	}

	ActiveHealth = FMath::Clamp(ActiveHealth - DamageAmount, 0.0f, MaxHealth); // clamps to a min of 0

	if(ActiveHealth <= 0)
	{
		if (GameModeReference)
		{
			GameModeReference->DeathHandle(GetOwner());
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Health Component of %s (ID: %s) has no reference to GameMode"),*GetOwner()->GetActorLabel(), *GetOwner()->GetName());
		}
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveHealth < MaxHealth && ActiveHealth > 0.f)
	{
		// percentage-based healing per second
		float amt_regen = (HealthRecoveryRate * 0.01f * MaxHealth) * DeltaTime;
		ActiveHealth += amt_regen;
	}
	
	float DeltaStaminaDrain = StaminaDrainRate * DeltaTime;
	float DeltaStaminaRecovery = StaminaRecoveryRate * DeltaTime;
	
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if(bIsSprinting)
		{
			if (ActiveStamina - DeltaStaminaDrain <= MinSprintStamina) // if nearly out of stamina, update status
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			
			ActiveStamina -= DeltaStaminaDrain; // deduct stamina
			if (owningCharacter) // can still sprint
				owningCharacter->canSprintStamina = true;
		}
		else // not sprinting
		{
			if (ActiveStamina + DeltaStaminaDrain >= MaxStamina)
				ActiveStamina = MaxStamina; // stop stamina increasing past the maximum threshold
			
			else
				ActiveStamina += DeltaStaminaRecovery; // increase stamina, like normal
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bIsSprinting)
		{
			if (ActiveStamina - DeltaStaminaDrain <= 0.f) // if stamina reaches 0 or below
			{
				// cannot sprint if there is no stamina
				if (owningCharacter)
					owningCharacter->canSprintStamina = false;
				
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				ActiveStamina = 0; // clamp it to 0, so as not to be negative
				
			}
			else
			{
				ActiveStamina -= DeltaStaminaDrain; // reduce like normal
				// can still sprint (stamina is available)
				if (owningCharacter)
					owningCharacter->canSprintStamina = true;
			}
		}
		else
		{
			if (ActiveStamina + DeltaStaminaDrain >= MinSprintStamina) // stam has reached the recovery mark
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				ActiveStamina += DeltaStaminaRecovery; // increase in accordance
			}
			else
			{
				ActiveStamina += DeltaStaminaRecovery; // keep increasing, remain in the same status
			}
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bIsSprinting)
		{
			if (owningCharacter)
				owningCharacter->canSprintStamina = false; // just in-case, ensure they cannot sprint
			ActiveStamina = 0.f; // clamp at 0
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_Recovering); // recovery occurs once the player stops trying to sprint
			ActiveStamina += DeltaStaminaRecovery;
		}
		// cannot sprint when exhausted
		if (owningCharacter)
			owningCharacter->canSprintStamina = false;
		break;
	case EStaminaStatus::ESS_Recovering:
		if (ActiveStamina + DeltaStaminaDrain >= MinSprintStamina) // if passed the threshold for the required recovery amount to sprint again
		{
			if (owningCharacter)
				owningCharacter->canSprintStamina = true; // regained ability to sprint
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			ActiveStamina += DeltaStaminaRecovery;
		}
		else
		{
			ActiveStamina += DeltaStaminaRecovery;
		}
		break;
		
	default:
		/* nothing */;
	}
}

void UHealthComponent::OnSprintStatusChange(bool bIsSprinting_)
{
	bIsSprinting = bIsSprinting_;
}




