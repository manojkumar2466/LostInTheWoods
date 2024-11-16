// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTINTHEWOODS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float health;

	UPROPERTY(EditAnywhere)
	float maxHealth;

	UPROPERTY(EditAnywhere)
	float stamina = 100;

	UPROPERTY(EditAnywhere)
	float maxStamina = 100;

	UPROPERTY(EditAnywhere)
	float minStaminaToAttack = 10;

	UPROPERTY(EditAnywhere)
	float minStaminaForRageAttack = 10;

	UPROPERTY(EditAnywhere)
	float MinStaminaToDodge = 15;

	UPROPERTY(EditAnywhere)
	float staminaIncreaseRate = 2;

	UPROPERTY(EditAnywhere)
	float minStaminaToRun = 10;

	UPROPERTY(EditAnywhere)
	float useStaminaToRun = 1;

	UPROPERTY(EditAnywhere)
	int souls;

	UPROPERTY(EditAnywhere)
	int life;


public:		
	


	void ReceiveDamage(float damage);

	float GetHealthPercent();

	float GetStaminaPercent();
	float GetMinStaminaToAttack();
	float GetMinStaminaToDodge();
	float GetStaminsIncreaseRate();
	float GetMinStaminaToRun();
	float GetStaminaRateToRun();
	void RegenerateStamina(float DeltaTime);
	void UseStamina(float value);

	bool IsAlive();

	void AddSouls(int value);

	void AddHealth(int value);
	FORCEINLINE int GetSoulsCount() { return souls; }
	FORCEINLINE int GetLifeCount() { return life; }
		
};
