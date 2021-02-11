// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapItem.h"
#include "DefaultPlayerCharacter.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

ATrapItem::ATrapItem()
{
	Damage = 15.f;
}

void ATrapItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		ADefaultPlayerCharacter* Player =  Cast<ADefaultPlayerCharacter>(OtherActor); // cast to see if the colliding actor is the player
		if (Player)
		{
			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			
			Player->HealthComponent->DecrementHealth(Damage);
			Destroy();
		}
	}

}

void ATrapItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
