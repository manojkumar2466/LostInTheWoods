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

	FORCEINLINE ECharacterWeaponEquipState GetCharacterState() { return characterWeaponState; }

	FORCEINLINE void SetOverlapingItem(class AItem* item){ overlappingItem= item;}


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	class AItem* overlappingItem;

	//Monatge section
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* swordAttackMontage;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack();

	//Montage section
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();


private:


	UPROPERTY(EditDefaultsOnly, Category=Components)
	class USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, Category= Component)
	class UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	class AWeapon* inHandWeapon;


	UPROPERTY(VisibleAnywhere, Category = Combat)
	ECharacterWeaponEquipState characterWeaponState= ECharacterWeaponEquipState::ECWES_UnEquipped;


	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess= "true"), Category = Combat)
	ECharacterActionState actionState = ECharacterActionState::ECAS_Unoccupied;



	// input functions start
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	
	//input functions end
	


};
