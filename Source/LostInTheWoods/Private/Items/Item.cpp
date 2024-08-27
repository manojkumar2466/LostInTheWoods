// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(mesh);
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
	FString otherActorName = OtherActor->GetName();
	GEngine->AddOnScreenDebugMessage(1, 5 ,FColor::Blue, otherActorName);
	UE_LOG(LogTemp, Warning, TEXT("AItem::OnSphereOverlapStart"));
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("AItem::OnSphereOverlapEnd"));
}



// Called every frame
void AItem::Tick(float DeltaTime)
{
	if (!bStopHovering) {
		sinValue += DeltaTime;
		float deltaZ = sinAmplitude * FMath::Sin(waveSpeed * sinValue);
		Super::Tick(DeltaTime);
		AddActorWorldOffset(FVector(0.f, 0.f, deltaZ));
		AddActorWorldRotation(FRotator(0.f, spinRate * DeltaTime, 0.f));

	}
	

}

