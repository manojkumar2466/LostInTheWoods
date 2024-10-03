// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

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

	healthComponent = CreateDefaultSubobject <UHealthComponent >(TEXT("HealthComp"));

	healthBarWidgetComponet = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarWidgetComp"));
	healthBarWidgetComponet->SetupAttachment(GetRootComponent()); 

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	pawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	if (pawnSensing)
	{
		pawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
		pawnSensing->SightRadius = 4000.f;
		pawnSensing->SetPeripheralVisionAngle(75.f);
	}
}

void AEnemy::GetHit_Implementation(const FVector& impactPoint)
{
	if (healthComponent && healthComponent->IsAlive())
	{
		HandleHealthBarWidgetVisibility(true);
		HitDirection(impactPoint);
	}
	else
	{
		
		OnDeath();
	}
	
	if (hitFleshSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, hitFleshSFX, impactPoint);
	}
	if (bloodVFX) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodVFX, impactPoint);
	
	DrawDebugSphere(GetWorld(), impactPoint, 8.f, 32.f, FColor::Blue, false, 5.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HandleHealthBarWidgetVisibility(false);
	AIController = Cast<AAIController>(GetController());

	MoveToTarget(patrolTarget, patrolAcceptanceRadius);
	
}

void AEnemy::PlayHitReactMonatge()
{
	PlayMontage(hitReactMontage, FName("FromBack"));
}

void AEnemy::PlayDeathMontage()
{
	TArray<FName> sections;
	sections.Add("Death1");
	sections.Add("Death2");
	sections.Add("Death3");
	
	int sectionIndex = FMath::FRandRange(0, 2);

	//TEnumAsByte<EDeathStatus> Pose(sectionIndex+1);
	characterDeathStatus = StaticCast<EDeathStatus>(sectionIndex + 1);
	PlayMontage(deathMontage, sections[sectionIndex]);
}

void AEnemy::PlayMontage(UAnimMontage* montage, FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName,montage);
	}
}

void AEnemy::OnDeath()
{
	PlayDeathMontage();
	HandleHealthBarWidgetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::HandleHealthBarWidgetVisibility(bool isVisible)
{
	if (healthBarWidgetComponet)
	{
		healthBarWidgetComponet->SetVisibility(isVisible);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsInRange(combatTarget, CombatRadius))
	{
		 combatTarget = nullptr;			
		 HandleHealthBarWidgetVisibility(false);
	}
	
	if (IsInRange(patrolTarget, patrolRadius))
	{
		ChangePatrolPoint();
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemy::OnPatrolTimerFinished, FMath::RandRange(1.f,5.f));
		
	}

	
		
	
	
	

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::HitDirection(const FVector& impactPoint)
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

bool AEnemy::IsInRange(AActor* target, double radius)
{
	if (!target)
	{
		return false;
	}
	double distance = (target->GetActorLocation() - GetActorLocation()).Size();
	return distance<radius;
}

void AEnemy::ChangePatrolPoint()
{
	TArray<AActor*> nextPatrolPointsTargetList;
	for (auto newPatrolPoint : patrolTargetPoints)
	{
		if (newPatrolPoint != patrolTarget) {

			nextPatrolPointsTargetList.AddUnique(newPatrolPoint);
		}
	}

	int patrolPointIndex = FMath::RandRange(0, nextPatrolPointsTargetList.Num() - 1);

	patrolTarget = nextPatrolPointsTargetList[patrolPointIndex];	
	
	
}

void AEnemy::MoveToTarget(AActor* targetActor, double acceptanceRadius)
{
	if (!AIController ||!targetActor)
	{
		return;
	}
	FAIMoveRequest moveRequest;
	moveRequest.SetGoalActor(targetActor);
	moveRequest.SetAcceptanceRadius(acceptanceRadius);
	AIController->MoveTo(moveRequest);
}

void AEnemy::OnPatrolTimerFinished()
{
	MoveToTarget(patrolTarget, patrolAcceptanceRadius);
}

void AEnemy::OnPawnSeen(APawn* Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("saw pawn"));
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (healthComponent && healthBarWidgetComponet) {

		healthComponent->ReceiveDamage(DamageAmount);
		healthBarWidgetComponet->SetHealthBarPercent(healthComponent->GetHealthPercent());
		combatTarget = EventInstigator->GetPawn();
	}
	
	return DamageAmount;
}

