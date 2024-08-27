// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorAnimInstance.h"
#include "Warrior.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	warrior = Cast<AWarrior>(TryGetPawnOwner());
	if (warrior) {
		characterMovementComponent = warrior->GetCharacterMovement();
	}
}

void UWarriorAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);
	if (characterMovementComponent) {
		speed= UKismetMathLibrary::VSizeXY(characterMovementComponent->Velocity);
		isFalling = characterMovementComponent->IsFalling();
		characterState = warrior->GetCharacterState();
	}
}
