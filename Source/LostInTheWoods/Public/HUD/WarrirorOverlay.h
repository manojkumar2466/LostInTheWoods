// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarrirorOverlay.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API UWarrirorOverlay : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta= (BindWidget))
	class UTextBlock* SoulText;

public:

	void SetHealthProgressBar(float percent);

	void SetStaminaProgressBar(float percent);

	
};
