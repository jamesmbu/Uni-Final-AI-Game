// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDefaultPlayerAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	ACharacterBase* owningCharacter = Cast<ACharacterBase>(Pawn);
	if (owningCharacter)
	{
		owningCharacter->bPlayerAttackStance.AddDynamic(this, &UDefaultPlayerAnimInstance::OnPlayerSwitchStance);
	}
}

void UDefaultPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
		MovementSpeed = LateralVelocity.Size(); // magnitude of the vector

		// Calculate Direction
		Direction = CalculateDirection(Velocity, Pawn->GetActorRotation());
		bInTheAir = Pawn->GetMovementComponent()->IsFalling();
	}
}

void UDefaultPlayerAnimInstance::CPPUpdateAnimation()
{

}

void UDefaultPlayerAnimInstance::OnPlayerSwitchStance(bool bInAttackStance_)
{
	bInAttackStance = bInAttackStance_;
}
