// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f; // maximum health value
	UPROPERTY(VisibleAnywhere)
	float ActiveHealth = 0.f; // current health
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DamageCushioning = 0.f; // damage reduction
	AFYP_K1811535GameModeBase* GameModeReference;
public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Taking damage
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);

public:	
	
		
};
