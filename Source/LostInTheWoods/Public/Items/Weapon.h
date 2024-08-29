// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon();

	void Equip(USceneComponent* parentMesh, FName socketName);

	FORCEINLINE class UBoxComponent* GetWeaponBoxCollider() { return boxCollider; }

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	//box trace
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxCollider;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* startTrace;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* endTrace;
};
