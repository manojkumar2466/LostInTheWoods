// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HealthComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapon.h"
#include "Items/Soul.h"

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
	shieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	shieldMesh->SetupAttachment(GetRootComponent());
	shieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	shieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Tags.Add("Enemy");

	/*weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	weaponMesh->SetupAttachment(GetRootComponent());
	weaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HandleHealthBarWidgetVisibility(false);
	AIController = Cast<AAIController>(GetController());

	patrolTarget = patrolTargetPoints[FMath::RandRange(0, patrolTargetPoints.Num() - 1)];
	StartPatroling();

	
	FAttachmentTransformRules transformrules(EAttachmentRule::SnapToTarget, true);
	shieldMesh->AttachToComponent(GetMesh(), transformrules, shieldSocketName);

	//weaponMesh->AttachToComponent(GetMesh(), transformrules, FName("InHandWeaponSocket"));

	if (weaponClass)
	{
		inHandWeapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);
		inHandWeapon->Equip(GetMesh(), weaponSocketName, this, this);

	}

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
		return;
	}

	if (currentState > EEnemyState::EES_Patrolling)
	{
		CombatCheck();
	}
	else
	{
		PatrolCheck();
	}
	

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
		ClearTimer(attackTimer);
		LoseInterestChasingPlayer();
		if (!IsEngaged())
		{
			StartPatroling();
		}
		
	}
	else if (!IsInRange(combatTarget, attackingRadius) && currentState!= EEnemyState::EES_Chasing)
	{
		ClearTimer(patrolTimer);
		ClearTimer(attackTimer);
		if (!IsEngaged())
		{
			StartChasing(combatTarget);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("chasing from combat check"));
		
	}
	else if (CanAttack())
	{		
		ClearTimer(attackTimer);
		ClearTimer(patrolTimer);
		StartAttackTimer();
		UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	}
}


void AEnemy::HandleHealthBarWidgetVisibility(bool isVisible)
{
	if (healthBarWidgetComponet)
	{
		healthBarWidgetComponet->SetVisibility(isVisible);
	}
}

void AEnemy::StartAttackTimer()
{
	currentState = EEnemyState::EES_Attacking;
	GetWorldTimerManager().SetTimer(attackTimer,this,&AEnemy::Attack,FMath::RandRange(attackMinTime,attackMaxTime));
}

void AEnemy::StartPatroling()
{
	currentState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = patrolWalkSpeed;
	MoveToTarget(patrolTarget, patrolAcceptanceRadius);
}

void AEnemy::SpawnSoulOrHealth()
{
	UWorld* world = GetWorld();
	if (world && attributeComponent && soulClass)
	{
		FVector spawnLocation = GetActorLocation() + FVector(0.f, 0.f, 25.f);

		int selectionIndex = FMath::RandRange(0, 1);
		TEnumAsByte<ECollectable> collectableType(selectionIndex);
		ASoul* spawnedSoulorHealth = world->SpawnActor<ASoul>(soulClass, spawnLocation, GetActorRotation());
		if (spawnedSoulorHealth && collectableType == ECollectable::EC_Soul) {
			
			spawnedSoulorHealth->SetSouls(attributeComponent->GetSoulsCount());
			spawnedSoulorHealth->collectbleType = ECollectable::EC_Soul;
		}
		else if (spawnedSoulorHealth && collectableType == ECollectable::EC_Health)
		{
			
			spawnedSoulorHealth->SetHealth(attributeComponent->GetLifeCount());
			spawnedSoulorHealth->SetItemVFX(spawnedSoulorHealth->GetHealthVFX());
			spawnedSoulorHealth->collectbleType = ECollectable::EC_Health;
			
		}
		
		
	}
}

bool AEnemy::CanAttack()
{
	return IsInRange(combatTarget, attackingRadius) && currentState != EEnemyState::EES_Attacking && currentState!=EEnemyState::EES_Dead && currentState!= EEnemyState::EES_Engaged;
}

void AEnemy::ClearTimer(FTimerHandle timer)
{
	GetWorldTimerManager().ClearTimer(timer);
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

bool AEnemy::IsEngaged()
{
	return currentState== EEnemyState::EES_Engaged;
}

bool AEnemy::IsDead()
{
	return currentState==EEnemyState::EES_Dead;
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
	bool canChase = currentState== EEnemyState::EES_Patrolling
		&& pawn->ActorHasTag(FName("Warrior"));

	if(canChase)
	{
		combatTarget = pawn;
		ClearTimer(patrolTimer);
		StartChasing(combatTarget);
		UE_LOG(LogTemp, Warning, TEXT("saw pawn"));
	}
	

}

void AEnemy::OnDeath()
{
	Super::OnDeath();	
	currentState = EEnemyState::EES_Dead;
	HandleWeaponBoxCollision(ECollisionEnabled::NoCollision);
	HandleHealthBarWidgetVisibility(false);
	DisableCapusleCollider();
	ClearTimer(attackTimer);
	GetCharacterMovement()->bOrientRotationToMovement = false;	
    GetWorldTimerManager().SetTimer(soulSpawnTimer,this, &AEnemy::SpawnSoulOrHealth,1.5f, false );
}


void AEnemy::PatrolCheck()
{
	if (IsInRange(patrolTarget, patrolRadius))
	{
		ChangePatrolPoint();
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemy::OnPatrolTimerFinished, FMath::RandRange(patrolWaitTimeMin, patrolwaitTimeMax));

	}
}

//Protected Functions
void AEnemy::Attack()
{
	Super::Attack();
	if (!combatTarget)
	{
		return;
	}
	currentState = EEnemyState::EES_Engaged;
	PlayAttackMontage(attackMontage);

}

void AEnemy::AttackEnd()
{
	Super::AttackEnd();
	currentState = EEnemyState::EES_NoState;
}

void AEnemy::LoseInterestChasingPlayer()
{
	combatTarget = nullptr;
	HandleHealthBarWidgetVisibility(false);
	UE_LOG(LogTemp, Warning, TEXT("lose interest"));
}



void AEnemy::StartChasing(AActor* targetActor)
{
	currentState = EEnemyState::EES_Chasing;
	combatTarget = targetActor;
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	MoveToTarget(targetActor,combatAcceptanceRadius);
	UE_LOG(LogTemp, Warning, TEXT("Chasing"));
}

void AEnemy::TakeHit_Implementation(FVector pointOfImpact, AActor* imHittingActor)
{

	Super::TakeHit_Implementation(pointOfImpact, imHittingActor);
	if (IsAlive())
	{

		HandleHealthBarWidgetVisibility(true);
	}
	ClearTimer(patrolTimer);
	ClearTimer(attackTimer);
	StopMontage(attackMontage);
	if (IsInRange(patrolTarget, attackingRadius))
	{
		if (IsAlive())
		{
			
			StartAttackTimer();
			UE_LOG(LogTemp, Error, TEXT("Started Attacking"));
		}
	}
	else
	{
		float distnace = (patrolTarget->GetActorLocation() - GetActorLocation()).Size();
		UE_LOG(LogTemp, Error, TEXT("Not in Attacking Range and distance is:%f "), distnace);
	}

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount,DamageEvent, EventInstigator,DamageCauser);
	if (attributeComponent && healthBarWidgetComponet) {

		healthBarWidgetComponet->SetHealthBarPercent(attributeComponent->GetHealthPercent());
		
	}
	
	
	if (IsInRange(combatTarget, attackingRadius))
	{
		currentState = EEnemyState::EES_Attacking;
	}
	else if( !IsInRange(combatTarget, attackingRadius))
	{
		StartChasing(combatTarget);
	}
	
	return DamageAmount;
}

