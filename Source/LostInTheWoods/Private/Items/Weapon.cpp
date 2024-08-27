// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Warrior.h"

AWeapon::AWeapon()
{
}

void AWeapon::Equip(USceneComponent* parentMesh, FName socketName)
{
	weaponState = EWeaponState::EWS_Equiped;
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	mesh->AttachToComponent(parentMesh, attachmentRules, FName("WeaponSocketR"));
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
