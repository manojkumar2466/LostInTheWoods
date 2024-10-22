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

	TArray<AActor*> ignoreActors;

	void Equip(USceneComponent* parentMesh, FName socketName, AActor* newOwner, APawn* newInstigator);

	FORCEINLINE class UBoxComponent* GetWeaponBoxCollider() { return boxCollider; }

protected:

	virtual void BeginPlay() override;

	
	UFUNCTION()
	void OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	//box trace

	UPROPERTY(EditAnywhere)
	FVector boxTraceExtends= FVector(5.f);
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxCollider;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* startTrace;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* endTrace;

	UPROPERTY(EditAnywhere)
	float damage = 20.f;

	void StartBoxTrace(FHitResult& hitResult);
};
