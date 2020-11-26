// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class FYP_K1811535_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	// Trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UBoxComponent* TriggerBox;

	// Physical floor switch mesh; the character steps on this
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* FloorSwitch;

	// Door that will open; the effect
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* Door;

	// Initial location for the door
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialDoorLocation;

	// Initial location for the floor switch
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overlap events
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Movement of meshes
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaisePad();
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerPad();

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdatePadLocation(float Z);

	FTimerHandle FloorSwitchTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	float CloseDoorDelay;

	void CloseDoor();

	bool bActorOnSwitch;
};
