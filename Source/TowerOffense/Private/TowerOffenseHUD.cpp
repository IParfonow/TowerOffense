// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerOffenseHUD.h"
#include "Blueprint/UserWidget.h"

void ATowerOffenseHUD::BeginPlay()
{
	Super::BeginPlay();

	if(HealthBarWidgetClass)
	{
		HealthBar = CreateWidget(GetGameInstance(), HealthBarWidgetClass);
		HealthBar->AddToViewport();
	}

	if(ShellsCounterClass)
	{
		ShellsCounter = CreateWidget(GetGameInstance(), ShellsCounterClass);
		ShellsCounter->AddToViewport();
	}

	if(EnemiesCountClass)
	{
		EnemiesCounter = CreateWidget(GetGameInstance(), EnemiesCountClass);
		EnemiesCounter->AddToViewport();
	}
}

void ATowerOffenseHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	VisibilityTime += DeltaSeconds;
	if(VisibilityTime >= VisibilityTimeLimit)
	{
		SetWidgetVisibility(false);
	}
}

void ATowerOffenseHUD::SetWidgetVisibility(bool bIsVisible)
{
	ESlateVisibility Type = (bIsVisible) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	HealthBar->SetVisibility(Type);
	ShellsCounter->SetVisibility(Type);
}

void ATowerOffenseHUD::InitializeHUDState()
{
	SetWidgetVisibility(true);
	VisibilityTime = 0.f;
}
