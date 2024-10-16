// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	healthComponent = CreateDefaultSubobject <UHealthComponent >(TEXT("HealthComp"));

}
 
// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::Attack()
{
	
	
}

bool ABaseCharacter::CanAttack()
{
	return false;
}





void ABaseCharacter::HandleWeaponBoxCollision(ECollisionEnabled::Type collisionType)
{
	if (inHandWeapon && inHandWeapon->GetWeaponBoxCollider())
	{
		inHandWeapon->GetWeaponBoxCollider()->SetCollisionEnabled(collisionType);
			if (collisionType == ECollisionEnabled::NoCollision) {
				inHandWeapon->ignoreActors.Empty();
			}
	}
	
}

void ABaseCharacter::OnDeath()
{
	PlayDeathMontage();
}

void ABaseCharacter::PlayAttackMontage()
{
	int sectionSelectionIndex = FMath::RandRange(0, attackMontageSectionNames.Num() - 1);
	PlayMontage(attackMontage, attackMontageSectionNames[sectionSelectionIndex]);
}

void ABaseCharacter::PlayDeathMontage()
{

}

void ABaseCharacter::PlayHitReactMonatge(FName sectionName)
{
	PlayMontage(hitReactMontage, sectionName);
}

void ABaseCharacter::PlayMontage(UAnimMontage* montage, FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName, montage);
	}
}





