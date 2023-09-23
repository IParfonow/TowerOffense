// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

void ATankPlayerController::SetPlayerEnabledState(bool bIsEnabled)
{
	if(bIsEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}

void ATankPlayerController::HandleEndGame(bool IsPlayersWin)
{
	ShowEndGameWidget(IsPlayersWin ? WinClassWidget : LoseClassWidget);
	SetPlayerEnabledState(false);
}

void ATankPlayerController::ShowEndGameWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if(IsValid(WidgetClass))
	{
		UUserWidget* EndGameWidget = CreateWidget<UUserWidget>(GetGameInstance(),WidgetClass);
		if(IsValid(EndGameWidget))
		{
			EndGameWidget->AddToViewport();
		}
	}
}
