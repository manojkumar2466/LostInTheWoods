// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamagableInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class LOSTINTHEWOODS_API ABaseCharacter : public ACharacter, public IDamagableInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	
protected:

	UPROPERTY(EditAnywhere, Category= MontageSection)
	TArray<FName> attackMontageSectionNames;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* bloodVFX;

	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere)
	TArray<FName> deathMontageSectionNames;

	int deathPoseIndex;

	UPROPERTY(EditAnywhere, Category = SFX)
	USoundBase* hitFleshSFX;

	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* hitReactMontage;
		
	UPROPERTY(VisibleAnywhere, Category = Combat)
	class AWeapon* inHandWeapon;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* healthComponent;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* attackMontage;

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	void DisableCapusleCollider();

	virtual void GetHit_Implementation(const FVector& impactPoint, AActor* hittingActor);

	UFUNCTION(BlueprintCallable)
	void HandleWeaponBoxCollision(ECollisionEnabled::Type collisionType);

	void HitDirection( const FVector& impactPoint);

	bool IsAlive();
	virtual void OnDeath();

	virtual void PlayAttackMontage();

	void PlayBloodVFX(const FVector& impactPoint);
	virtual void PlayDeathMontage();
	virtual void PlayHitReactMonatge(FName sectionName);

	void PlayHitSound( const FVector &impactPoint);


	virtual void PlayMontage(UAnimMontage* montage, FName sectionName);
};
