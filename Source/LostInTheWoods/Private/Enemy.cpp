// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

}

void AEnemy::GetHit(const FVector& impactPoint)
{
	HitDirection( impactPoint);
	
	DrawDebugSphere(GetWorld(), impactPoint, 8.f, 32.f, FColor::Blue, false, 5.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMonatge()
{
	PlayMontage(hitReactMontage, FName("FromBack"));
}

void AEnemy::PlayMontage(UAnimMontage* montage, FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName,montage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::HitDirection(FVector impactPoint)
{
	FVector forwardVector = GetActorForwardVector();
	FVector toHit = (impactPoint - GetActorLocation()).GetSafeNormal();
	FVector normalZToHit = FVector(toHit.X, toHit.Y, GetActorLocation().GetSafeNormal().Z);
	double dotProduct= FVector::DotProduct(forwardVector, normalZToHit);
	double cosTheta = FMath::Acos(dotProduct);
	double degrees = FMath::RadiansToDegrees(cosTheta);
	

	FVector crossProductVector= FVector::CrossProduct(forwardVector, normalZToHit);
	if (crossProductVector.Z < 0) {
		degrees *= -1.f;
	}
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(TEXT("Angle:%f"), degrees));
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(), GetActorLocation()+forwardVector*60,40.f,FLinearColor::Blue, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + normalZToHit * 60, 40.f, FLinearColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(),  GetActorLocation()+crossProductVector*100.f, 40.f, FLinearColor::Black, 5.f);

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
	PlayMontage(hitReactMontage, hitReactionSectionName);

}

