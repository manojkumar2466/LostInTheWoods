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

	


private:
	
	
	 
	UPROPERTY(EditAnywhere)
	class UHealthBarWidgetComponent* healthBarWidgetComponet;

	

	UPROPERTY(EditAnywhere)
	double chaseRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double chaseWalkSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	AActor* patrolTarget;

	UPROPERTY(EditAnywhere)
	double patrolRadius = 100.f;

	UPROPERTY(EditAnywhere)
	double patrolWalkSpeed = 150.f;

	UPROPERTY(EditAnywhere)
	double patrolAcceptanceRadius = 100.f;

	UPROPERTY(EditAnywhere)
	double combatAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere)
	double attackingRadius = 150.f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* shieldMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> weaponClass;

	UPROPERTY(EditAnywhere)
	FName weaponSocketName;

	UPROPERTY(EditAnywhere)
	FName shieldSocketName;




	UPROPERTY(EditAnywhere)
	TArray<AActor*> patrolTargetPoints;

	class AAIController* AIController;

	FTimerHandle patrolTimer;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* pawnSensing;

	bool CanAttack();
	void ClearTimer(FTimerHandle timer);


	bool IsInRange(AActor* target, double radius);
	bool IsEngaged();
	bool IsDead();

	void ChangePatrolPoint();

	void MoveToTarget(AActor* targetActor, double acceptanceRadius);

	void OnPatrolTimerFinished();

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void StartChasing(AActor* targetActor);

	void CombatCheck();

	void PatrolCheck();

	

	


protected:
	
	UPROPERTY(EditAnywhere)
	float attackMinTime;

	UPROPERTY(EditAnywhere)
	float attackMaxTime;

	
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState currentState = EEnemyState::EES_Patrolling;

	FTimerHandle attackTimer;

	
	virtual void Attack() override;

	
	virtual void AttackEnd() override;

	void LoseInterestChasingPlayer();

	virtual void OnDeath() override;
	void HandleHealthBarWidgetVisibility(bool isVisible);


	void StartAttackTimer();
	void StartPatroling();

	



public:
	
	virtual void GetHit_Implementation(const FVector& impactPoint, AActor* hittingActor) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
