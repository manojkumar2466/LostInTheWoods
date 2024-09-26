// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidgetComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidgetComponent::SetHealthBarPercent(float percent)
{
	if (!healthBarWidget) {

         healthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());

	}
	if (healthBarWidget && healthBarWidget->healthBar)
	{
		healthBarWidget->healthBar->SetPercent(percent);
	}
	
}
