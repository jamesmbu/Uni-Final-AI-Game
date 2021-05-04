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

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_Recovering UMETA(DisplayName = "Recovering"),
	ESS_MAX UMETA(DisplayName = "DefaultMax"),
};

class AFYP_K1811535GameModeBase;
class ACharacterBase;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth; // maximum health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ActiveHealth; // current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DamageCushioning; // damage reduction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthRecoveryRate; // rate of health recovery, % per second
	
	void DecrementHealth(float Amount);
	void HandleDeath();
	
	/* STAMINA */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina; // maximum health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float ActiveStamina; // current health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus; // enum for stamina assessment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate; // rate of stamina drain
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRecoveryRate; // rate of stamina recovery
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinSprintStamina; // value which denotes a threshold where stamina is nearly depleted
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; };
	UPROPERTY(BlueprintReadOnly)
	bool bIsSprinting;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Taking damage
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float DamageAmount, 
		const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnSprintStatusChange(bool bIsSprinting_); // function called when listener hears change in sprint state

	UPROPERTY()
	ACharacterBase* owningCharacter; // reference to owning character

	
};
