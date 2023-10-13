
#include "TankPlayerController.h"
#include "TankPawn.h"


float ATankPlayerController::GetEndWidgetTimeDuration() const
{
	return EndWidgetTimeDuration;
}

void ATankPlayerController::SetPlayerEnabledState(bool bIsEnabled)
{
	ATankPawn* ControlledTank = Cast<ATankPawn>(GetPawn());
	if (!IsValid(ControlledTank))
	{
		return;
	}
	
	if (bIsEnabled)
	{
		EnableInput(this);
		ControlledTank->AddMappingContextToInput();
	}
	else
	{
		DisableInput(this);
	}
}

void ATankPlayerController::HandleEndGame(bool ArePlayersWin)
{
	ShowEndGameWidget(ArePlayersWin ? WinClassWidget : LoseClassWidget);
	SetPlayerEnabledState(false);
}


void ATankPlayerController::ShowEndGameWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if(IsValid(WidgetClass))
	{
		EndGameWidget = CreateWidget(GetGameInstance(),WidgetClass);
		if(IsValid(EndGameWidget))
		{
			EndGameWidget->AddToViewport();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,
				&ATankPlayerController::HideEndGameWidget,EndWidgetTimeDuration,false);
		}
	}
}

void ATankPlayerController::HideEndGameWidget()
{
	if(IsValid(EndGameWidget))
	{
		EndGameWidget->RemoveFromParent();
		EndGameWidget = nullptr;
	}
}
