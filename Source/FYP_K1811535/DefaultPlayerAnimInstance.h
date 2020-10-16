// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DefaultPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Transient, Blueprintable, HideCategories = AnimInstance, BlueprintType)
class FYP_K1811535_API UDefaultPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable, Category = "Animation Properties")
	void CPPUpdateAnimation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bInTheAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	
};
