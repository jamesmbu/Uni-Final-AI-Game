// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	Ems_Normal UMETA(DisplayName = "Normal"),
	Ems_Sprinting UMETA(DisplayName = "Sprinting"),
	Ems_Max UMETA(DisplayName = "LastEnum"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeStance, bool, bInAttackStance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprint, bool, bIsSprinting);

class UHealthComponent;
class AWeapon;
class AItem;
class UAnimMontage;
UCLASS()
class FYP_K1811535_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	/*~~~~~~~~~~  Input-related functions  ~~~~~~~~~~*/
	// Interaction
	bool bInteractKeyDown;
	void InteractBegin();
	void InteractEnd();
	// Main action (LMB)
	void MainAction(); // left mouse button
	void MainActionEnd();
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool bMainActionKeyDown;
	// Animations
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* CombatMontage;
	// Player Stance
	void SetStance();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isAttackStance;
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnChangeStance bPlayerAttackStance;
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnSprint bPlayerSprinting;
	
	// Sprinting
	void SprintBegin();
	void SprintEnd();
	bool canSprint;
	UPROPERTY(BlueprintReadOnly)
	bool canSprintStamina;
	bool isSprinting;

	// Movement & Looking
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bAttacking;
	
	/* Weapon Equipping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	AItem* ActiveOverlappingItem;
	
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }
	/*~~~~~~~~~~  Components  ~~~~~~~~~~*/
	// Camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	/*~~~~~~~~~~  Move & Look Adjustment Variables  ~~~~~~~~~~*/
	// Movement speed when jogging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move & Look Variables")
	float BaseJogSpeed = 400.f;
	// How much faster the player goes when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move & Look Variables")
	float SprintMultiplier = 1.5f;
	// How much slower the player goes when in backwards motion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move & Look Variables")
	float BackwardsMotionMultiplier = 0.6f;
	// For gamepad analogue stick aiming- rate at which rotation will occur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move & Look Variables")
	float RotationRate = 70.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
