// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnumTypes.h"
#include "WarriorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API UWarriorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	class AWarrior* warrior;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* characterMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float speed;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isFalling;
	UPROPERTY(BlueprintReadOnly, Category = "States")
	ECharacterWeaponEquipState characterState;


};
