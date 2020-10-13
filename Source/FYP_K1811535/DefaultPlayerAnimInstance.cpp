// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDefaultPlayerAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
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

		bInTheAir = Pawn->GetMovementComponent()->IsFalling();
	}
}

void UDefaultPlayerAnimInstance::CPPUpdateAnimation()
{

}
