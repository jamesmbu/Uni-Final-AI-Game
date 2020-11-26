// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningZone"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomSpawnPoint()
{
	FVector Extent = SpawningZone->GetScaledBoxExtent();
	FVector Origin = SpawningZone->GetComponentLocation();

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void ASpawnVolume::SpawnThePawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		if (World)
		{
			APawn* TheSpawnedPawn = World->SpawnActor<APawn>(ToSpawn, Location, FRotator(0.f), SpawnParams);
			
		}
	}
}

