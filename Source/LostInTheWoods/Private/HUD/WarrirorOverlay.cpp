// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WarrirorOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWarrirorOverlay::SetHealthProgressBar(float percent)
{
	HealthProgressBar->SetPercent(percent);
}

void UWarrirorOverlay::SetStaminaProgressBar(float percent)
{
	StaminaProgressBar->SetPercent(percent);
}

void UWarrirorOverlay::SetSoulCount(float count)
{
	SoulText->SetText((FText::FromString(FString::Printf(TEXT("%f"), count))));
}
