// Fill out your copyright notice in the Description page of Project Settings.


#include "Warrior.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AWarrior::AWarrior()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	springArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	springArm->SetupAttachment(GetRootComponent());
	camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	camera->SetupAttachment(springArm);


}

// Called when the game starts or when spawned
void AWarrior::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWarrior::Attack()
{
	
	if (CanAttack()) {
		PlayAttackMontage();
		actionState = ECharacterActionState::ECAS_Attacking;
	}
}

void AWarrior::PlayAttackMontage()
{
	int32 randomIndex = FMath::RandRange(0, 1);

	TArray<FName> sections;
	sections.Add("Attack1");
	sections.Add("Attack2");
	PlayMontage(swordAttackMontage, sections[randomIndex]);

	
}

void AWarrior::SetPlayerActionToUnoccupied()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}

void AWarrior::HandleWeaponBoxCollision(ECollisionEnabled::Type collisionType)
{
	inHandWeapon->GetWeaponBoxCollider()->SetCollisionEnabled(collisionType);
	if (collisionType == ECollisionEnabled::NoCollision) {
		inHandWeapon->ignoreActors.Empty();
	}
}

void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarrior::GetHit_Implementation(const FVector& impactPoint)
{
	DrawDebugSphere(GetWorld(), impactPoint, 8.f, 32.f, FColor::Blue, false, 5.f);
}

// Called to bind functionality to input
void AWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AWarrior::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AWarrior::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AWarrior::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AWarrior::LookUp);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &AWarrior::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AWarrior::Attack);

}


void AWarrior::MoveForward(float value)
{
	if (Controller&& value != 0 && actionState== ECharacterActionState::ECAS_Unoccupied) {
		FRotator controllerRotation = Controller->GetControlRotation();//COntroller does have forward vector, sp we are taking controllers rotation
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);

		FVector forwardDirectionVector= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);//x axis because we want forward vector after app;ying rotation

		AddMovementInput(forwardDirectionVector, value);
	}
	
}

void AWarrior::MoveRight(float value)
{
	if (Controller && value != 0 && actionState == ECharacterActionState::ECAS_Unoccupied) {
		FRotator controllerRotation = Controller->GetControlRotation();
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);
		FVector rightVector= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(rightVector, value);

	}
}

void AWarrior::Turn(float value)
{
	AddControllerYawInput(value);
}

void AWarrior::LookUp(float value)
{
	AddControllerPitchInput(value);
}

bool AWarrior::CanAttack()
{
	return actionState == ECharacterActionState::ECAS_Unoccupied && characterWeaponState == ECharacterWeaponEquipState::ECWES_Equipped;
}

bool AWarrior::CanDisarm()
{
	return  inHandWeapon && characterWeaponState == ECharacterWeaponEquipState::ECWES_Equipped && actionState==ECharacterActionState::ECAS_Unoccupied;
}

bool AWarrior::CanEquipWeapon()
{
	return  inHandWeapon && characterWeaponState != ECharacterWeaponEquipState::ECWES_Equipped && actionState == ECharacterActionState::ECAS_Unoccupied;
}

void AWarrior::PlayMontage(UAnimMontage* montage, FName SectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (animInstance)
	{
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(SectionName, montage);

	}
}


//public functions

void AWarrior::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(overlappingItem);
	if (weapon) {
		characterWeaponState = ECharacterWeaponEquipState::ECWES_Equipped;
		inHandWeapon = weapon;
		weapon->Equip(GetMesh(), FName("WeaponSocketR"));
		overlappingItem = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Overlaping weapon"));
	}
	else if (CanDisarm()) {

		PlayMontage(swordEquipMontage, FName("Disarm"));
		characterWeaponState = ECharacterWeaponEquipState::ECWES_UnEquipped;
		actionState = ECharacterActionState::ECAS_DisarmWeapon;
	}
	else if (CanEquipWeapon()) {
		PlayMontage(swordEquipMontage, FName("Equip"));
		characterWeaponState = ECharacterWeaponEquipState::ECWES_Equipped;
		actionState = ECharacterActionState::ECAS_EquipingWeapon;

	}
		

}

void AWarrior::EquipWeaponFromBack()
{
	AttachComponentToMesh(FName("WeaponSocketR"));
}
void AWarrior::Disarm()
{
	AttachComponentToMesh(FName("WeaponSocket"));
}

void AWarrior::AttachComponentToMesh(FName socketName)
{
	FAttachmentTransformRules attachmentTransferRules(EAttachmentRule::SnapToTarget, true);
	inHandWeapon->AttachToComponent(GetMesh(), attachmentTransferRules, socketName );
}





