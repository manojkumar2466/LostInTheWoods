// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.h"
#include "Item.generated.h"

UCLASS()
class LOSTINTHEWOODS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
	virtual void OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void PlayPickupSound();
	void SpawnPickupVFX();

	UPROPERTY(EditAnywhere)
	USoundBase* pickupSound;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphere;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	float waveSpeed=2;
	UPROPERTY(EditAnywhere)
	float sinAmplitude=1;
	UPROPERTY(EditAnywhere)
	float sinValue=2;
	UPROPERTY(EditAnywhere)
	float spinRate=2;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* itemEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* pickupVFX;
	

	EWeaponState weaponState = EWeaponState::EWS_Hovering;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
