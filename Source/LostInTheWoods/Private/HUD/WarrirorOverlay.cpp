// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WarrirorOverlay.h"
#include "Components/ProgressBar.h"

void UWarrirorOverlay::SetHealthProgressBar(float percent)
{
	HealthProgressBar->SetPercent(percent);
}

void UWarrirorOverlay::SetStaminaProgressBar(float percent)
{
	StaminaProgressBar->SetPercent(percent);
}
