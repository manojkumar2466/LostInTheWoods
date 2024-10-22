// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"


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

void ABaseCharacter::AttackEnd()
{

}

void ABaseCharacter::DisableCapusleCollider()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

bool ABaseCharacter::IsAlive()
{
	return healthComponent && healthComponent->IsAlive();
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

void ABaseCharacter::PlayBloodVFX(const FVector& impactPoint)
{
	if (bloodVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodVFX, impactPoint);
	}
}

void ABaseCharacter::PlayDeathMontage()
{
	int sectionNameIndex = FMath::RandRange(0, deathMontageSectionNames.Num() - 1);
	deathPoseIndex = sectionNameIndex;
	PlayMontage(deathMontage, deathMontageSectionNames[sectionNameIndex]);
}

void ABaseCharacter::PlayHitReactMonatge(FName sectionName)
{
	PlayMontage(hitReactMontage, sectionName);
}

void ABaseCharacter::PlayHitSound(const FVector& impactPoint)
{
	if (hitFleshSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, hitFleshSFX, impactPoint);
	}
}

void ABaseCharacter::PlayMontage(UAnimMontage* montage, FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName, montage);
	}
}





