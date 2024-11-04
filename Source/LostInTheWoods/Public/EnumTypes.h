#pragma once


UENUM(BlueprintType)
enum class ECharacterWeaponEquipState : uint8
{
	ECWES_UnEquipped UMETA(DisplayName= "NoWeapon"),
	ECWES_Equipped UMETA(DisplayName="HaveWeapon")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_Unoccupied UMETA(DisplayName="Unoccupied"),
	ECAS_HitReact UMETA(DisplayName="Hit React"),
	ECAS_Attacking UMETA(DisplayName="Attacking"),
	ECAS_EquipingWeapon UMETA(DisplayName="Equiping weapon"),
	ECAS_DisarmWeapon UMETA(DisplayName = "Disarm weapon"),
	ECAS_Dodge UMETA(DisplayName="Dodge"),
	ECAS_Dead UMETA(DisplayName = "Dead")
};


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Hovering,
	EWS_Equiped
};

UENUM(BlueprintType)
enum class EEnemyDeathPose : uint8
{
	EDS_DeathPose1 UMETA(DisplayName = "Deathpose1"),
	EDS_DeathPose2 UMETA(DisplayName = "Deathpose2"),
	EDS_DeathPose3 UMETA(DisplayName = "Deathpose3")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	
	EES_Patrolling,
	EES_Chasing,
	EES_Attacking,
	EES_Engaged,
	EES_Dead, 
	EES_NoState
};