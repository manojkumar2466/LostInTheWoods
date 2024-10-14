// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "Character/BaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class LOSTINTHEWOODS_API AEnemy : public ABaseCharacter
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
	
	
	 
	UPROPERTY(EditAnywhere)
	class UHealthBarWidgetComponent* healthBarWidgetComponet;

	UPROPERTY(VisibleAnywhere)
	AActor* combatTarget;

	UPROPERTY(EditAnywhere)
	double chaseRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	AActor* patrolTarget;

	UPROPERTY(EditAnywhere)
	double patrolRadius = 100.f;

	UPROPERTY(EditAnywhere)
	double patrolAcceptanceRadius = 100.f;

	UPROPERTY(EditAnywhere)
	double combatAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere)
	double attackingRadius = 150.f;


	UPROPERTY(EditAnywhere)
	TArray<AActor*> patrolTargetPoints;

	class AAIController* AIController;

	FTimerHandle patrolTimer;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* pawnSensing;

	UPROPERTY(VisibleAnywhere)
	EEnemyState currentState = EEnemyState::EES_Patrolling;

	void HitDirection(const FVector& hitResult);

	bool IsInRange(AActor* target, double radius);

	void ChangePatrolPoint();

	void MoveToTarget(AActor* targetActor, double acceptanceRadius);

	void OnPatrolTimerFinished();

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void StartChasing(AActor* targetActor);

	void CombatCheck();

	void PatrolCheck();


protected:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDeathStatus characterDeathStatus = EDeathStatus::EDS_Alive;

	

	virtual void PlayDeathMontage() override;
	virtual void OnDeath() override;
	void HandleHealthBarWidgetVisibility(bool isVisible);




public:
	virtual void GetHit_Implementation(const FVector& impactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
