// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Pickup.h"
#include "EnumTypes.h"
#include "Warrior.generated.h"


UCLASS()
class LOSTINTHEWOODS_API AWarrior : public ACharacter, public IPickup
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarrior();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Input Function start
	void EKeyPressed();
	//Input Function End

	FORCEINLINE ECharacterWeaponEquipState GetCharacterState() { return characterState; }

	FORCEINLINE void SetOverlapingItem(class AItem* item){ overlappingItem= item;}


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	class AItem* overlappingItem;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack();


private:


	UPROPERTY(EditDefaultsOnly, Category=Components)
	class USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, Category= Component)
	class UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	class AWeapon* inHandWeapon;


	UPROPERTY(VisibleAnywhere, Category = Combat)
	ECharacterWeaponEquipState characterState= ECharacterWeaponEquipState::ECWES_UnEquipped;

	UPROPERTY(EditAnywhere, Category= Combat)
	UAnimMontage* swordAttackMontage;



	// input functions start
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	
	//input functions end
	


};
