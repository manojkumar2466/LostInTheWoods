// Fill out your copyright notice in the Description page of Project Settings.


#include "Warrior.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWarrior::AWarrior()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetGenerateOverlapEvents(true);	
	springArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	springArm->SetupAttachment(GetRootComponent());
	camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	camera->SetupAttachment(springArm);
	Tags.Add(FName("Warrior"));

}

// Called when the game starts or when spawned
void AWarrior::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWarrior::Attack()
{
	Super::Attack();
	if (CanAttack()) {
		PlayAttackMontage();
		actionState = ECharacterActionState::ECAS_Attacking;
	}
}



void AWarrior::SetPlayerActionToUnoccupied()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}


void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarrior::GetHit_Implementation(const FVector& impactPoint)
{
	DrawDebugSphere(GetWorld(), impactPoint, 8.f, 32.f, FColor::Blue, false, 5.f);
	PlayHitSound(impactPoint);
	PlayBloodVFX(impactPoint);
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




//public functions

void AWarrior::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(overlappingItem);
	if (weapon) {
		characterWeaponState = ECharacterWeaponEquipState::ECWES_Equipped;
		inHandWeapon = weapon;
		weapon->Equip(GetMesh(), FName("WeaponSocketR"), this, this);
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





