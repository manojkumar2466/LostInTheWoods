// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Warrior.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/DamagableInterface.h"

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

void AWeapon::Equip(USceneComponent* parentMesh, FName socketName)
{
	weaponState = EWeaponState::EWS_Equiped;
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	mesh->AttachToComponent(parentMesh, attachmentRules, FName("WeaponSocketR"));
	sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlapStart);
}

void AWeapon::OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapStart(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep, SweepResult);
	
	AWarrior* warrior = Cast<AWarrior>(OtherActor);
	if (warrior) {
		warrior->SetOverlapingItem(this);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnSphereOverlapStart"));
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AWarrior* warrior = Cast<AWarrior>(OtherActor);
	if (warrior) {
		warrior->SetOverlapingItem(nullptr);

	}
}

void AWeapon::OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector start = startTrace->GetComponentLocation();
	const FVector end = endTrace->GetComponentLocation();
	FHitResult hitResult;
	TArray<AActor*> ignoreActorList;
	ignoreActorList.Add(this);
	for (AActor* Actor : ignoreActors) {
		ignoreActorList.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		start,
		end,
		FVector(5.f, 5.f, 5.f),
		startTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoreActorList,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true
		);

	if (hitResult.GetActor())
	{
		IDamagableInterface* hitActor = Cast<IDamagableInterface>(hitResult.GetActor());
		if (hitActor) {
			hitActor->GetHit(hitResult.ImpactPoint);
		}		
		ignoreActors.AddUnique(hitResult.GetActor());
	}

}
