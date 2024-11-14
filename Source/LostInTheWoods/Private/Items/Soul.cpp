// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/Pickup.h"



void ASoul::OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickup* pickupInterface = Cast<IPickup>(OtherActor);
	if (pickupInterface)
	{
		pickupInterface->AddSoulOrHealth(this);
		SpawnPickupVFX();
		PlayPickupSound();
		Destroy();
	}
}
