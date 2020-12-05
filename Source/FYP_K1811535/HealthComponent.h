// Fill out your copyright notice in the Description page of Project Settings.

/*
 *
 *	A custom component which gives health and stamina functionality
 * 
 */
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AFYP_K1811535GameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FYP_K1811535_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	
	
	/* GAME MODE REFERENCE */
	AFYP_K1811535GameModeBase* GameModeReference;
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	/* HEALTH */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth; // maximum health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ActiveHealth; // current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DamageCushioning; // damage reduction

	void DecrementHealth(float Amount);
	void HandleDeath();
	
	/* STAMINA */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina; // maximum health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float ActiveStamina; // current health

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Taking damage
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);

public:	
	
		
};
