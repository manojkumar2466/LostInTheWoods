// Fill out your copyright notice in the Description page of Project Settings.


#include "Warrior.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	if (actionState == ECharacterActionState::ECAS_Unoccupied) {
		PlayAttackMontage();
		actionState = ECharacterActionState::ECAS_Attacking;
	}
}

void AWarrior::PlayAttackMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	int32 randomIndex = FMath::RandRange(0, 1);

	TArray<FName> sections;
	sections.Add("Attack1");
	sections.Add("Attack2");

	if (animInstance)
	{
		animInstance->Montage_Play(swordAttackMontage);
		animInstance->Montage_JumpToSection(sections[randomIndex], swordAttackMontage);

	}
}

void AWarrior::AttackEnd()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}

void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if (Controller&& value != 0) {
		FRotator controllerRotation = Controller->GetControlRotation();//COntroller does have forward vector, sp we are taking controllers rotation
		FRotator yawRotation(0.f, controllerRotation.Yaw, 0.f);

		FVector forwardDirectionVector= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);//x axis because we want forward vector after app;ying rotation

		AddMovementInput(forwardDirectionVector, value);
	}
	
}

void AWarrior::MoveRight(float value)
{
	if (Controller && value != 0) {
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


//public functions

void AWarrior::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(overlappingItem);
	if (weapon) {
		characterWeaponState = ECharacterWeaponEquipState::ECWES_Equipped;
		weapon->Equip(GetMesh(), FName("WeaponSocketR"));
	}
}

// Called every frame

