// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "FYP_K1811535/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
	// Make controller rotation not effect the character's rotation; only the camera will move.
	bUseControllerRotationYaw = false; // < this is the only line that matters
	bUseControllerRotationPitch = false; // < + |,< these are just in case
	bUseControllerRotationRoll = false;

	// Set character to move in direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;
}




// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacterBase::Jump);
	PlayerInputComponent->BindAction(TEXT("Focus"), EInputEvent::IE_Pressed, this, &ACharacterBase::SetFocused);
	PlayerInputComponent->BindAction(TEXT("Focus"), EInputEvent::IE_Released, this, &ACharacterBase::SetUnfocused);

}
/*
 * Input
 */
void ACharacterBase::SetFocused()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ACharacterBase::SetUnfocused()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ACharacterBase::MoveForward(float AxisValue)
{
	if (Controller && AxisValue != 0.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); // get yaw of the controller's rotation vector
		const FVector DirectionToMove = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // get X from the rotation matrix of the 'YawRotation'
		AddMovementInput(DirectionToMove, AxisValue);
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}

}

void ACharacterBase::MoveRight(float AxisValue)
{
	if (Controller && AxisValue != 0)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); // get yaw of the controller's rotation vector
		const FVector DirectionToMove = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // get Y from the rotation matrix of the 'YawRotation'
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

/*
 * Input end
 */


