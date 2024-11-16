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
	bool CanRun();
	//Input Function End

	FORCEINLINE ECharacterWeaponEquipState GetCharacterState() { return characterWeaponState; }
 
	void SetOverlapingItem(class AItem* item) override;
	virtual void AddSoulOrHealth(class ASoul* soul) override;

	FORCEINLINE ECharacterActionState GetWarriorActionState() { return actionState; }
	//HitInterface
	virtual void TakeHit_Implementation(FVector pointOfImpact, AActor* imHittingActor) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	class AItem* overlappingItem;

	ECharacterAttackType attackType= ECharacterAttackType::ECAT_NormalAttack;

	UPROPERTY(BlueprintReadOnly)
	bool canRun = false;
	//Monatge section
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* dodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* swordEquipMontage;

	UPROPERTY(EditAnywhere, Category= Combat)
	UAnimMontage* RageAttackMontage;

	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* LowerAttackMontage;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Attack() override;
	void Dodge();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
	virtual void Jump() override;

	virtual void OnDeath() override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerActionToUnoccupied();

	void SwitchToRageAttacks();
	void SwitchToLowerAttacks();
	void SwitchToNormalAttacks();



private:

	UPROPERTY(EditAnywhere)
	float attackRadius=100.f;

	UPROPERTY(EditAnywhere)
	float warriorSightRadius =500;

	bool canUseLeftOverStaminaToRun = true;
	UPROPERTY(EditAnywhere)
	float walkSpeed=150.f;

	
	
	UPROPERTY(EditDefaultsOnly, Category=Components)
	class USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, Category= Component)
	class UCameraComponent* camera;



	UPROPERTY(VisibleAnywhere, Category = Combat)
	ECharacterWeaponEquipState characterWeaponState= ECharacterWeaponEquipState::ECWES_UnEquipped;


	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess= "true"), Category = Combat)
	ECharacterActionState actionState = ECharacterActionState::ECAS_Unoccupied;

	class UWarrirorOverlay* warriorOverlay;


	void CreateWarriorHUD();

	// input functions start
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void StartRun(float value);
	void StopRun();
	
	
	//input functions end

	//AttackFunction
	virtual bool CanAttack();
	bool CanDisarm();
	bool CanEquipWeapon();
	
	UFUNCTION(BlueprintCallable)
	bool IsEnemyInAttackRadius(AActor* enemy);

	UFUNCTION(BlueprintCallable)
	void EquipWeaponFromBack();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UAnimMontage* GetMontageForAttackType();
	void AttachComponentToMesh(FName socketName);
	UFUNCTION(BlueprintCallable)
	void Disarm();

	void UpdateHealthProgressBar();

	
	



};
