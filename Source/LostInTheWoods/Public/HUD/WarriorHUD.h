// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WarriorHUD.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API AWarriorHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	 TSubclassOf<class UWarrirorOverlay> warriorOverlayClass;

	 class UWarrirorOverlay* warriorOverlay;

public:

	FORCEINLINE class UWarrirorOverlay* GetWarriorOverlay() const { return warriorOverlay; }
	
};
