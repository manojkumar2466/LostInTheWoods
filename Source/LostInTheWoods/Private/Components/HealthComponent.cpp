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

bool UHealthComponent::IsAlive()
{
	return health>0.f;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

