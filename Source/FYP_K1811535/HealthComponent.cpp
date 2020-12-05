// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "FYP_K1811535/Modes/FYP_K1811535GameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100.f;
	ActiveHealth = MaxHealth;
	DamageCushioning = 0.f;

	MaxStamina = 100.f;
	ActiveStamina = MaxStamina;
	
	// ...
}

void UHealthComponent::DecrementHealth(float Amount)
{
	if (ActiveHealth - Amount <= 0.f)
		HandleDeath();
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
	
	GameModeReference = Cast<AFYP_K1811535GameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); // find the currently active game mode
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage); // when the owning actor calls OnTakeAnyDamage, the dynamic delegate will be called from this class using TakeDamage
	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount == 0)
	{
		return;
	}

	ActiveHealth = FMath::Clamp(ActiveHealth - DamageAmount, 0.0f, MaxHealth);

	if(ActiveHealth <= 0)
	{
		if (GameModeReference)
		{
			GameModeReference->DeathHandle(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Component of %s (ID: %s) has no reference to GameMode"),*GetOwner()->GetActorLabel(), *GetOwner()->GetName());
		}
	}
}


