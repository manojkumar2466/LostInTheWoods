// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"

#include "Warrior.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(mesh);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphere->SetupAttachment(GetRootComponent());
	sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapStart);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
	
}

void AItem::OnSphereOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarrior* warrior = Cast<AWarrior>(OtherActor);
	if (warrior) {
		warrior->SetOverlapingItem(this);

	}
	
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWarrior* warrior = Cast<AWarrior>(OtherActor);
	if (warrior) {
		warrior->SetOverlapingItem(nullptr);

	}
}



// Called every frame
void AItem::Tick(float DeltaTime)
{
	if (weaponState==EWeaponState::EWS_Hovering) {
		sinValue += DeltaTime;
		float deltaZ = sinAmplitude * FMath::Sin(waveSpeed * sinValue);
		Super::Tick(DeltaTime);
		AddActorWorldOffset(FVector(0.f, 0.f, deltaZ));
		AddActorWorldRotation(FRotator(0.f, spinRate * DeltaTime, 0.f));

	}
	

}

