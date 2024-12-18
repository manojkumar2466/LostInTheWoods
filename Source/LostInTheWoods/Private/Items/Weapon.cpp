// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/DamagableInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollider"));
	boxCollider->SetupAttachment(GetRootComponent());
	boxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	startTrace = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	startTrace->SetupAttachment(GetRootComponent());

	endTrace = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	endTrace->SetupAttachment(GetRootComponent());
	 
}

void AWeapon::Equip(USceneComponent* parentMesh, FName socketName, AActor* newOwner, APawn* newInstigator)
{
	SetOwner(newOwner);
	SetInstigator(newInstigator);
	weaponState = EWeaponState::EWS_Equiped;
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	mesh->AttachToComponent(parentMesh, attachmentRules, socketName);
	sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlapStart);
}



void AWeapon::OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	FHitResult hitResult;
	StartBoxTrace(hitResult);
	if (hitResult.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && hitResult.GetActor()->ActorHasTag(TEXT("Enemy")))
		{
			return;
		}
		IDamagableInterface* hitActor = Cast<IDamagableInterface>(hitResult.GetActor());
		if (hitActor) {
			UGameplayStatics::ApplyDamage(
				hitResult.GetActor(),
				damage, GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass());
			AActor* owner = GetOwner();
			hitActor->Execute_TakeHit(hitResult.GetActor(), hitResult.ImpactPoint, owner);
			//hitActor->Execute_GetHit(hitResult.GetActor(), hitResult.ImpactPoint, GetOwner());

		}
		ignoreActors.AddUnique(hitResult.GetActor());
	}
	

}

void AWeapon::StartBoxTrace(FHitResult& hitResult)
{
	const FVector start = startTrace->GetComponentLocation();
	const FVector end = endTrace->GetComponentLocation();

	TArray<AActor*> ignoreActorList;
	ignoreActorList.Add(this);
	ignoreActorList.Add(GetOwner());
	for (AActor* Actor : ignoreActors) {
		ignoreActorList.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		start,
		end,
		boxTraceExtends,
		startTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoreActorList,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true
	);

}
