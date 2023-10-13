// Fill out your copyright notice in the Description page of Project Settings.


#include "CountdownWidget.h"
#include "TowerOffenseGameMode.h"

void UCountdownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ATowerOffenseGameMode* TowerOffenseGameMode = Cast<ATowerOffenseGameMode>(GetWorld()->GetAuthGameMode());
	
	TowerOffenseGameMode->OnStartCountDown.AddDynamic(this, &UCountdownWidget::StartCountdown);
}

void UCountdownWidget::StartCountdown(float PrepareTime)
{
	CurrentCountdown = PrepareTime;
}