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

void ABaseCharacter::GetHit_Implementation(const FVector& impactPoint, AActor* hittingActor)
{
	

	PlayHitSound(impactPoint);
	PlayBloodVFX(impactPoint);

	DrawDebugSphere(GetWorld(), impactPoint, 8.f, 32.f, FColor::Blue, false, 5.f);
}

FVector ABaseCharacter::GetMotionwarpTranslationTargetLocation()
{
	if (!combatTarget)
	{
		return FVector();
	}

	FVector combatTargetLocation = combatTarget->GetActorLocation();
	FVector myLocation = GetActorLocation();

	FVector combatTOMe = (myLocation - combatTargetLocation).GetSafeNormal();

	combatTOMe *= motionwarpDistance;
	 
	return myLocation + combatTOMe;
	
}






FVector ABaseCharacter::GetMotionwarpTargetRotation()
{
	if (combatTarget)
	{
		return combatTarget->GetActorLocation();
	}
	return FVector();
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

void ABaseCharacter::HitDirection( const FVector& impactPoint)
{

	FVector forwardVector = GetActorForwardVector();
	FVector toHit = (impactPoint - GetActorLocation()).GetSafeNormal();
	FVector normalZToHit = FVector(toHit.X, toHit.Y, GetActorLocation().GetSafeNormal().Z);
	double dotProduct = FVector::DotProduct(forwardVector, normalZToHit);
	double cosTheta = FMath::Acos(dotProduct);
	double degrees = FMath::RadiansToDegrees(cosTheta);


	FVector crossProductVector = FVector::CrossProduct(forwardVector, normalZToHit);
	if (crossProductVector.Z < 0) {
		degrees *= -1.f;
	}
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(TEXT("Angle:%f"), degrees));
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + forwardVector * 60, 40.f, FLinearColor::Blue, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + normalZToHit * 60, 40.f, FLinearColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProductVector * 100.f, 40.f, FLinearColor::Black, 5.f);

	FName hitReactionSectionName("FromBack");
	if (degrees < 45 && degrees >= -45) {
		hitReactionSectionName = FName("FromFront");
	}
	else if (degrees >= 45 && degrees < 135) {
		hitReactionSectionName = FName("FromRight");
	}
	else if (degrees < -45 && degrees >= -135) {
		hitReactionSectionName = FName("FromLeft");
	}
	PlayHitReactMonatge(hitReactionSectionName);
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

void ABaseCharacter::StopMontage(UAnimMontage* montage)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance)
	{
		animInstance->Montage_Stop(0.25f,montage);
	}
}





