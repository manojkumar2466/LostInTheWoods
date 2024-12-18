// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API ASoul : public AItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	int soulValue=2;
	UPROPERTY(EditAnywhere)
	int healthValue = 5;
	
	

	

public:

	

	virtual void OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	FORCEINLINE int GetSoulValue() { return soulValue; }
	FORCEINLINE void SetSouls(int value) { soulValue = value; }
	FORCEINLINE int GetHealthValue() { return healthValue; }
	FORCEINLINE void SetHealth(int value) { healthValue = value; }
	FORCEINLINE class UNiagaraComponent* GetHealthVFX() { return redHealthVFX; }
};
