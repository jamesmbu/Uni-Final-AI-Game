// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "FYP_K1811535/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FYP_K1811535/Weapon.h"
#include "Animation/AnimInstance.h" 
#include "FYP_K1811535/MainPlayerController.h"
#include "FYP_K1811535/Characters/Enemy.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isAttackStance = false;
	isSprinting = false;
	canSprint = false;
	canSprintStamina = true;
	/*__________________________________________________________________________
	 * Create Components
	 */
	
	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 350.f; // follow distance
	CameraBoom->bUsePawnControlRotation = true; // rotation based on controller

	// Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // the boom is in charge of camera rotation

	// Create Health
	HealthComponent=CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	// __________________________________________________________________________
	
	// Make controller rotation not effect the character's rotation
	bUseControllerRotationYaw = false;

	// Set character to move in direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bHasCombatTarget = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), GetVelocity().Size()); //SPEED TEST
	//UE_LOG(LogTemp, Warning, TEXT("Control Rotation Yaw: %f"), Controller->GetControlRotation().Yaw);
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // macro to check validity of the input component

	// Movement bindings
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterBase::MoveRight);

	// 'Looking' bindings
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacterBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ACharacterBase::LookRight);

	// Binding for constant axis changes (i.e. when using a games controller)
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterBase::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &ACharacterBase::LookRightRate);

	// Action binding
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ACharacterBase::InteractBegin);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Released, this, &ACharacterBase::InteractEnd);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	PlayerInputComponent->BindAction(TEXT("Stance"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetStance);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacterBase::SprintBegin);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ACharacterBase::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACharacterBase::MainAction);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &ACharacterBase::MainActionEnd);
}
void ACharacterBase::InteractBegin()
{
	bInteractKeyDown = true;
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}

void ACharacterBase::InteractEnd()
{
	bInteractKeyDown = false;
	
}

void ACharacterBase::MainAction()
{
	bMainActionKeyDown = true;
	if (EquippedWeapon)
	{
		Attack();
	}
}

void ACharacterBase::MainActionEnd()
{
	bMainActionKeyDown = false;
}

void ACharacterBase::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			FName SectionName;
			float AnimationTimeMult = 1.0f;
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				SectionName = FName("Attack_1");
				AnimationTimeMult = 1.4f;
				break;
			case 1:
				SectionName = FName("Attack_2");
				AnimationTimeMult = 1.4f;
				break;
			default:
				;
			}
			AnimInstance->Montage_Play(CombatMontage, AnimationTimeMult);
			AnimInstance->Montage_JumpToSection(SectionName, CombatMontage);
		}
	}
	
}

void ACharacterBase::AttackEnd()
{
	bAttacking = false;
	if (bMainActionKeyDown)
	{
		Attack();
	}
}

/*
 * Input
 */
void ACharacterBase::SetStance()
{
	if (isAttackStance) // transition between stances: attack -> neutral
	{
		isAttackStance = false;
		bUseControllerRotationYaw = false;
	}
	else if (!isAttackStance) // transition between stances: neutral -> attack
	{
		isAttackStance = true;
		bUseControllerRotationYaw = true;
	}
	bPlayerAttackStance.Broadcast(isAttackStance);

}

void ACharacterBase::SprintBegin()
{
	if (canSprint && canSprintStamina)
	{
		isSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = BaseJogSpeed * SprintMultiplier;
		bPlayerSprinting.Broadcast(isSprinting);
	}
}

void ACharacterBase::SprintEnd()
{
	if (isSprinting) 
	{
		isSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseJogSpeed / SprintMultiplier;
		bPlayerSprinting.Broadcast(isSprinting);
	}
}

void ACharacterBase::MoveForward(float AxisValue)
{
	if (Controller && AxisValue != 0.f && !bAttacking)
	{
		if (AxisValue > 0) // Forward movement
		{
			canSprint = true;
			if (!isSprinting || !canSprintStamina) GetCharacterMovement()->MaxWalkSpeed = BaseJogSpeed; // ensure that forwards speed is correctly set in case it has been previously been reduced by walking backwards
		}
		else // Backward movement
		{
			canSprint = false; // cannot sprint backwards
			GetCharacterMovement()->MaxWalkSpeed = BaseJogSpeed * BackwardsMotionMultiplier; // reduced speed when walking backwards
		}
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); // get yaw of the controller's rotation vector
		const FVector DirectionToMove = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // get X from the rotation matrix of the 'YawRotation'
		SetActorRotation(YawRotation);
		AddMovementInput(DirectionToMove, AxisValue);
		
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	if (Controller && AxisValue != 0 && !bAttacking)
	{
		
		if (GetInputAxisValue(TEXT("MoveForward")) >= 0) // If movement direction is not backwards
		{
			GetCharacterMovement()->MaxWalkSpeed = BaseJogSpeed;
		}
		
		canSprint = false; // player cannot sprint when strafing
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); // get yaw of the controller's rotation vector
		const FVector DirectionToMove = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // get Y from the rotation matrix of the 'YawRotation'
		SetActorRotation(YawRotation); // reset actor's rotation to match the current control (yaw) rotation (important for maintaining back and side movement)
		AddMovementInput(DirectionToMove, AxisValue);
	}
}

void ACharacterBase::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ACharacterBase::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HealthComponent->DecrementHealth(DamageAmount);
	if (HealthComponent->ActiveHealth <= 0.f)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection("Death");
		}
	}
	return DamageAmount;
}

void ACharacterBase::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

/*
 * Input end
 */


