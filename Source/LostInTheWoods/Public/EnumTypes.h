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
	ECAS_Attacking UMETA(DisplayName="Attacking")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Hovering,
	EWS_Equiped
};
