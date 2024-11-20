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
	attributeComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Attribute"));
	
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
	if (combatTarget && combatTarget->ActorHasTag(FName("Dead")))
	{
		combatTarget = nullptr;
	}
	
}

void ABaseCharacter::AttackEnd()
{

}

void ABaseCharacter::DisableCapusleCollider()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::TakeHit_Implementation(FVector pointOfImpact, AActor* imHittingActor)
{
	
	if (IsAlive())
	{
		HitDirection(imHittingActor->GetActorLocation());

	}
	else
	{

		OnDeath();
	}

	PlayHitSound(pointOfImpact);
	PlayBloodVFX(pointOfImpact);

	
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
	 
	return combatTargetLocation + combatTOMe;
	
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
	return attributeComponent && attributeComponent->IsAlive();
}

void ABaseCharacter::OnDeath()
{	
		
	Tags.Empty();
	Tags.Add("Dead");
	PlayDeathMontage();
	TEnumAsByte<EEnemyDeathPose> characterDeathPose(deathPoseIndex);
	deathPose = characterDeathPose;
	
}

void ABaseCharacter::PlayAttackMontage(UAnimMontage* montage)
{
	if (!montage)
	{
		return;
	}
	int sectionSelectionIndex = FMath::RandRange(0, attackMontageSectionNames.Num() - 1);
	PlayMontage(montage, attackMontageSectionNames[sectionSelectionIndex]);
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

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	combatTarget = EventInstigator->GetPawn();
	if(attributeComponent)
	{
		attributeComponent->ReceiveDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("damage received"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("healthcomponent is null"));
	}
	return DamageAmount;
}

void ABaseCharacter::StopMontage(UAnimMontage* montage)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance)
	{
		animInstance->Montage_Stop(0.25f,montage);
	}
}





