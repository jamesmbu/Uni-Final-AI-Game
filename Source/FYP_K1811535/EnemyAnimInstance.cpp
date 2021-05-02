// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Characters/Enemy.h"
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

		Enemy = Cast<AEnemy>(Pawn);
	}
}
