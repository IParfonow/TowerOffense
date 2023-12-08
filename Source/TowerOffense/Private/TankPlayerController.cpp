
#include "TankPlayerController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "TowerOffenseGameMode.h"
#include "TankPawn.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ControlledPawn = Cast<ATurretPawn>(this->GetPawn());
	if(!ControlledPawn)
	 {
	 	UE_LOG(LogTemp, Warning, TEXT("Pawn didn't cast"));
	 }
}

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

void ATankPlayerController::LoadAssets()
{	
	const TArray<FSoftObjectPath> AssetsPaths = ControlledPawn->GetAssetsPaths(); 
	
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(AssetsPaths, FStreamableDelegate::CreateUObject(
		this, &ATankPlayerController::OnAssetsLoaded));
}

void ATankPlayerController::OnAssetsLoaded()
{
	DecreaseControllersCounter.Broadcast();
}

IAsyncLoadableObject::FOnAssetsLoaded& ATankPlayerController::GetOnAssetsLoadedEvent()
{
	return  DecreaseControllersCounter;
}

