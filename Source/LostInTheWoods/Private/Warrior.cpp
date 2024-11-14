// Fill out your copyright notice in the Description page of Project Settings.


#include "Warrior.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/WarriorHUD.h"
#include "Items/Soul.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/WarrirorOverlay.h"
#include "Components/HealthComponent.h"

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
	 
	CreateWarriorHUD();
}
void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canRun && warriorOverlay && attributeComponent)
	{
		attributeComponent->UseStamina(attributeComponent->GetStaminaRateToRun() * DeltaTime);
		warriorOverlay->SetStaminaProgressBar(attributeComponent->GetStaminaPercent());
		
		
	}
	if (!canRun && warriorOverlay && attributeComponent)
	{
		attributeComponent->RegenerateStamina(DeltaTime);
		warriorOverlay->SetStaminaProgressBar(attributeComponent->GetStaminaPercent());
	}

}

void AWarrior::Attack()
{
	Super::Attack();
	UAnimMontage* montage= GetMontageForAttackType();
	
		if (CanAttack() && attributeComponent->GetStaminaPercent() * 100 > attributeComponent->GetMinStaminaToAttack()) {
			PlayAttackMontage(montage);
			actionState = ECharacterActionState::ECAS_Attacking;
			attributeComponent->UseStamina(attributeComponent->GetMinStaminaToAttack());
			if (warriorOverlay)
			{
				warriorOverlay->SetStaminaProgressBar(attributeComponent->GetStaminaPercent());
			}
		}
	
}

void AWarrior::Dodge()
{
	if (attributeComponent->GetStaminaPercent()*100 < attributeComponent->GetMinStaminaToDodge() ||  actionState != ECharacterActionState::ECAS_Unoccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("health percent:%f\n minStaminaRequired To dodge:%f"), attributeComponent->GetStaminaPercent() * 100, attributeComponent->GetMinStaminaToDodge());
		return;
	}
	attributeComponent->UseStamina(attributeComponent->GetMinStaminaToDodge());
	actionState = ECharacterActionState::ECAS_Dodge;
	PlayMontage(dodgeMontage, FName("Dodge"));
	if (warriorOverlay)
	{
		warriorOverlay->SetStaminaProgressBar(attributeComponent->GetStaminaPercent());
	}
	
}

void AWarrior::DodgeEnd()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}

void AWarrior::Jump()
{
	if (attributeComponent->IsAlive())
	{
		Super::Jump();
	}
}

void AWarrior::OnDeath()
{
	Super::OnDeath();
	actionState = ECharacterActionState::ECAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void AWarrior::SetPlayerActionToUnoccupied()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}

void AWarrior::SwitchToRageAttacks()
{
	if (attackType != ECharacterAttackType::ECAT_RageAttack)
	{
		attackType = ECharacterAttackType::ECAT_RageAttack;
	}
}

void AWarrior::SwitchToLowerAttacks()
{
	if (attackType != ECharacterAttackType::ECAT_LowerAttck)
	{
		attackType = ECharacterAttackType::ECAT_LowerAttck;
	}
}

void AWarrior::SwitchToNormalAttacks()
{
	attackType = ECharacterAttackType::ECAT_NormalAttack;
}






void AWarrior::SetOverlapingItem(AItem* item)
{
	overlappingItem = item;
}

void AWarrior::AddSoulOrHealth(ASoul* soul)
{
	if (attributeComponent && warriorOverlay)
	{
		if (soul->collectbleType == ECollectable::EC_Soul)
		{
			attributeComponent->AddSouls(soul->GetSoulValue());
			warriorOverlay->SetSoulCount(attributeComponent->GetSouls());
		}
		else if (soul->collectbleType == ECollectable::EC_Health)
		{
			attributeComponent->AddHealth(soul->GetHealthValue());
			warriorOverlay->SetHealthProgressBar(attributeComponent->GetHealthPercent());
		}
		
	}
}




void AWarrior::TakeHit_Implementation(FVector pointOfImpact, AActor* imHittingActor)
{
	Super::TakeHit_Implementation(pointOfImpact, imHittingActor);
	HandleWeaponBoxCollision(ECollisionEnabled::NoCollision);
	if (attributeComponent->IsAlive())
	{
		HitDirection(imHittingActor->GetActorLocation());
		actionState = ECharacterActionState::ECAS_HitReact;
	}

}

float AWarrior::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UpdateHealthProgressBar();
	
	return DamageAmount;

}

// Called to bind functionality to input
void AWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AWarrior::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AWarrior::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AWarrior::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AWarrior::LookUp);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &AWarrior::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &AWarrior::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AWarrior::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), EInputEvent::IE_Pressed, this, &AWarrior::Dodge);
	PlayerInputComponent->BindAxis(FName("Run"),this,&AWarrior::StartRun);
	PlayerInputComponent->BindAction(FName("StopRun"), EInputEvent::IE_Released, this, &AWarrior::StopRun);
	PlayerInputComponent->BindAction(FName("RageAttack"), EInputEvent::IE_Pressed, this, &AWarrior::SwitchToRageAttacks);
	PlayerInputComponent->BindAction(FName("RageAttack"), EInputEvent::IE_Released, this, &AWarrior::SwitchToNormalAttacks);
	PlayerInputComponent->BindAction(FName("LowerAttack"), EInputEvent::IE_Pressed, this, &AWarrior::SwitchToLowerAttacks);
	PlayerInputComponent->BindAction(FName("LowerAttack"), EInputEvent::IE_Released, this, &AWarrior::SwitchToNormalAttacks);
	

}


void AWarrior::CreateWarriorHUD()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{

		AWarriorHUD* warriorHUD = Cast<AWarriorHUD>(playerController->GetHUD());

		if (warriorHUD)
		{
			warriorOverlay = warriorHUD->GetWarriorOverlay();

			if (warriorOverlay && attributeComponent)
			{
				warriorOverlay->SetHealthProgressBar(attributeComponent->GetHealthPercent());
				warriorOverlay->SetStaminaProgressBar(attributeComponent->GetStaminaPercent());
				warriorOverlay->SetSoulCount(5);
			}
		}
	}
}

void AWarrior::MoveForward(float value)
{
	if (Controller&& value != 0 && actionState== ECharacterActionState::ECAS_Unoccupied) {

		if (canRun)
		{
			GetCharacterMovement()->MaxWalkSpeed = runSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
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

void AWarrior::StartRun(float value)
{
	if (!attributeComponent)
	{
		return;
	}
	if (attributeComponent->GetStaminaPercent()*100 <=1.f)
	{
		canRun = false;
		return;
	}

	if (value > 0 && attributeComponent && attributeComponent->GetStaminaPercent()*100> attributeComponent->GetMinStaminaToRun())
	{
		canRun = true;
	}
	
	
}

void AWarrior::StopRun()
{
	canRun = false;
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

bool AWarrior::CanRun()
{
	return canRun;
}

void AWarrior::EquipWeaponFromBack()
{
	AttachComponentToMesh(FName("WeaponSocketR"));
}
void AWarrior::Disarm()
{
	AttachComponentToMesh(FName("WeaponSocket"));
}

void AWarrior::UpdateHealthProgressBar()
{
	if (warriorOverlay && attributeComponent)
	{
		warriorOverlay->SetHealthProgressBar(attributeComponent->GetHealthPercent());
	}
}

void AWarrior::HitReactEnd()
{
	actionState = ECharacterActionState::ECAS_Unoccupied;
}

UAnimMontage* AWarrior::GetMontageForAttackType()
{
	UAnimMontage* montage;
	switch (attackType)
	{

	case ECharacterAttackType::ECAT_RageAttack:
		montage = RageAttackMontage;
		break;
	case ECharacterAttackType::ECAT_LowerAttck:
		montage = LowerAttackMontage;
		break;
	default:
		montage = attackMontage;
		break;
	}
	return montage ;
}

void AWarrior::AttachComponentToMesh(FName socketName)
{
	FAttachmentTransformRules attachmentTransferRules(EAttachmentRule::SnapToTarget, true);
	inHandWeapon->AttachToComponent(GetMesh(), attachmentTransferRules, socketName );
}





