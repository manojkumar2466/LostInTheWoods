// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WarriorHUD.h"

#include "HUD/WarrirorOverlay.h"

void AWarriorHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world = GetWorld();
	APlayerController* playerController= world->GetFirstPlayerController();
	if (playerController && warriorOverlayClass)
	{
	  warriorOverlay= CreateWidget<UWarrirorOverlay>(playerController, warriorOverlayClass);
	  warriorOverlay->AddToViewport();

	}
	
}
