// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::ReceiveDamage(float damage)
{
	health = FMath::Clamp(health - damage, 0.f, maxHealth);
}

float UHealthComponent::GetHealthPercent()
{
	return health/maxHealth;
}

float UHealthComponent::GetStaminaPercent()
{
	return stamina/maxStamina;
}

float UHealthComponent::GetMinStaminaToAttack()
{
	return minStaminaToAttack;
}

float UHealthComponent::GetMinStaminaToDodge()
{
	return MinStaminaToDodge;
}

float UHealthComponent::GetStaminsIncreaseRate()
{
	return staminaIncreaseRate;
}

float UHealthComponent::GetMinStaminaToRun()
{
	return minStaminaToRun;
}

float UHealthComponent::GetStaminaRateToRun()
{
	return useStaminaToRun;
}

void UHealthComponent::RegenerateStamina(float DeltaTime)
{
	stamina = FMath::Clamp(stamina + staminaIncreaseRate * DeltaTime, 0, maxStamina);
}

void UHealthComponent::UseStamina(float value)
{
	stamina =  FMath::Clamp(stamina- value, 0, maxStamina);
}

bool UHealthComponent::IsAlive()
{
	return health>0.f;
}

void UHealthComponent::AddSouls(int value)
{
	souls += value;
}

void UHealthComponent::AddHealth(int value)
{
	health += value;
}




// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

