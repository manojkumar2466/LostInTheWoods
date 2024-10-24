// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Pickup.h"
#include "EnumTypes.h"
#include "Character/BaseCharacter.h"
#include "Warrior.generated.h"


UCLASS()
class LOSTINTHEWOODS_API AWarrior : public ABaseCharacter, public IPickup
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


	//HitInterface
	virtual void GetHit_Implementation(const FVector& impactPoint, AActor* hittingActor) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	class AItem* overlappingItem;

	//Monatge section
	

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* swordEquipMontage;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerActionToUnoccupied();

	


private:


	UPROPERTY(EditDefaultsOnly, Category=Components)
	class USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, Category= Component)
	class UCameraComponent* camera;



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

	//AttackFunction
	virtual bool CanAttack();
	bool CanDisarm();
	bool CanEquipWeapon();
	
	
	UFUNCTION(BlueprintCallable)
	void EquipWeaponFromBack();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	void AttachComponentToMesh(FName socketName);
	UFUNCTION(BlueprintCallable)
	void Disarm();



};
