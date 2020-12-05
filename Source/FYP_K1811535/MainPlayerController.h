// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class FYP_K1811535_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	/* References to UMG assets */
	// from the editor
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> HUDOverlayAsset;
	// the instantiated widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;
	
};
