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

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HandleHealthBarWidgetVisibility(false);
	AIController = Cast<AAIController>(GetController());

	patrolTarget = patrolTargetPoints[FMath::RandRange(0, patrolTargetPoints.Num() - 1)];
	MoveToTarget(patrolTarget, patrolAcceptanceRadius);

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentState > EEnemyState::EES_Patrolling)
	{
		CombatCheck();
	}
	else
	{
		PatrolCheck();
	}

}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

void AEnemy::CombatCheck()
{
	if (!IsInRange(combatTarget, chaseRadius))
	{
		combatTarget = nullptr;
		HandleHealthBarWidgetVisibility(false);
		currentState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
		MoveToTarget(patrolTarget, patrolAcceptanceRadius);
		UE_LOG(LogTemp, Warning, TEXT("lose interest"));
	}
	else if (!IsInRange(combatTarget, attackingRadius) && currentState!= EEnemyState::EES_Chasing)
	{
		StartChasing(combatTarget);
		UE_LOG(LogTemp, Warning, TEXT("chasing from combat check"));
		
	}
	else if (IsInRange(combatTarget, attackingRadius) && currentState != EEnemyState::EES_Attacking)
	{
		currentState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("Attacking"));
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

void AEnemy::HandleHealthBarWidgetVisibility(bool isVisible)
{
	if (healthBarWidgetComponet)
	{
		healthBarWidgetComponet->SetVisibility(isVisible);
	}
}

void AEnemy::HitDirection(const FVector& impactPoint)
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

bool AEnemy::IsInRange(AActor* target, double radius)
{
	if (!target)
	{
		return false;
	}
	double distance = (target->GetActorLocation() - GetActorLocation()).Size();
	return distance < radius;
}

void AEnemy::MoveToTarget(AActor* targetActor, double acceptanceRadius)
{
	if (!AIController || !targetActor)
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

void AEnemy::OnPawnSeen(APawn* pawn)
{
	if(currentState == EEnemyState::EES_Chasing || currentState== EEnemyState::EES_Attacking)
	{
		return;
	}
	if (pawn->ActorHasTag(FName("Warrior")))
	{

		combatTarget = pawn;
		StartChasing(combatTarget);
		UE_LOG(LogTemp, Warning, TEXT("saw pawn"));	

	}

}

void AEnemy::OnDeath()
{
	Super::OnDeath();
	PlayDeathMontage();
	HandleHealthBarWidgetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemy::PatrolCheck()
{
	if (IsInRange(patrolTarget, patrolRadius))
	{
		ChangePatrolPoint();
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemy::OnPatrolTimerFinished, FMath::RandRange(1.f, 5.f));

	}
}



void AEnemy::PlayDeathMontage()
{
	Super::SetPlayerDefaults();
	TArray<FName> sections;
	sections.Add("Death1");
	sections.Add("Death2");
	sections.Add("Death3");
	
	int sectionIndex = FMath::FRandRange(0, 2);

	//TEnumAsByte<EDeathStatus> Pose(sectionIndex+1);
	characterDeathStatus = StaticCast<EDeathStatus>(sectionIndex + 1);
	PlayMontage(deathMontage, sections[sectionIndex]);
}






void AEnemy::StartChasing(AActor* targetActor)
{
	currentState = EEnemyState::EES_Chasing;
	combatTarget = targetActor;
	GetWorldTimerManager().ClearTimer(patrolTimer);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	MoveToTarget(targetActor,combatAcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("Chasing"));
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (healthComponent && healthBarWidgetComponet) {

		healthComponent->ReceiveDamage(DamageAmount);
		healthBarWidgetComponet->SetHealthBarPercent(healthComponent->GetHealthPercent());
		
	}
	combatTarget = EventInstigator->GetPawn();
	StartChasing(combatTarget);
	return DamageAmount;
}

