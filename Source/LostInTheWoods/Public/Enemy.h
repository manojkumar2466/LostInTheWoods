// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamagableInterface.h"
#include "EnumTypes.h"
#include "Enemy.generated.h"

UCLASS()
class LOSTINTHEWOODS_API AEnemy : public ACharacter, public IDamagableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	


private:
	void HitDirection(const FVector& hitResult);
	
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* healthComponent;
	 
	UPROPERTY(EditAnywhere)
	class UHealthBarWidgetComponent* healthBarWidgetComponet;

	AActor* combatTarget;


protected:

	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* hitReactMontage;

	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere, Category = SFX)
	USoundBase* hitFleshSFX;
	UPROPERTY(EditAnywhere, Category = VFX)

	UParticleSystem* bloodVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDeathStatus characterDeathStatus = EDeathStatus::EDS_Alive;

	double CombatRadius = 500.f;

	void PlayHitReactMonatge();
	void PlayDeathMontage();
	void PlayMontage(UAnimMontage* montage, FName sectionName);
	void OnDeath();
	void HandleHealthBarWidgetVisibility(bool isVisible);




public:
	virtual void GetHit_Implementation(const FVector& impactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
