// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class LOSTINTHEWOODS_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
private:
	class UHealthBarWidget* healthBarWidget;

public:

	void SetHealthBarPercent(float percent);

	
};
