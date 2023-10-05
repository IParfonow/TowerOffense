// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerOffenseGameMode.h"
#include "TurretPawn.h"
#include "TankPawn.h"
#include "TowerPawn.h"
#include "Kismet/GameplayStatics.h"

ATowerOffenseGameMode::ATowerOffenseGameMode()
{
	DefaultPawnClass = ATankPawn::StaticClass();
}

void ATowerOffenseGameMode::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ChangeGameState(EGameState::MainMenu);
}

void ATowerOffenseGameMode::ChangeGameState(EGameState NewState)
{
	CurrenGameState = NewState;
	switch (CurrenGameState)
	{
	case EGameState::MainMenu:
		ShowMainMenu();
		break;
		
	case EGameState::Playing:
		DelayStart(TimeBeforeStart);
		break;
		
	case EGameState::Paused:
		//pause code
		break;

	case EGameState::GameOver:
		EndGame();
		break;		
	}
}


void ATowerOffenseGameMode::AddPawnToArray(ATurretPawn* Pawn)
{
	if(ATowerPawn* TowerPawn = Cast<ATowerPawn>(Pawn))
	{
		Enemies.Add(TowerPawn);
	}
	else if(ATankPawn* PlayerPawn = Cast<ATankPawn>(Pawn))
	{
		Players.Add(PlayerPawn);
	}
}

void ATowerOffenseGameMode::OnPawnDeath(ATurretPawn* DeadPawn)
{
	ATankPlayerController*  DeadTankPlayerController = Cast<ATankPlayerController>(DeadPawn->GetController());
	if(IsValid(DeadTankPlayerController))
	{
		if(ATankPawn* TankPawn = Cast<ATankPawn>(DeadPawn))
		{
			Players.Remove(TankPawn);
			if(Players.Num() == 0)
			{
				DeadTankPlayerController->HandleEndGame(false);
				ChangeGameState(EGameState::GameOver);
			}
		}
	}
	
	if(IsValid(TankPlayerController))
	{
		if(ATowerPawn* TowerPawn = Cast<ATowerPawn>(DeadPawn))
		{
			Enemies.Remove(TowerPawn);
			if(Enemies.Num() == 0)
			{
				TankPlayerController->HandleEndGame(true);
				GetWorldTimerManager().SetTimer(RestartTimerHandle, [this]()
				{
					ChangeGameState(EGameState::GameOver);
				}, TankPlayerController->GetEndWidgetTimeDuration(), false);
			}
		}
	}
	DeadPawn->Destroy();
}

void ATowerOffenseGameMode::DelayStart(float DelayTime)
{
	RemoveCurrentWidget();
	if(IsValid(PrepareWidgetClass))
	{
		CurrentMenuWidget = CreateWidget(GetGameInstance(), PrepareWidgetClass);
		if(IsValid(CurrentMenuWidget))
		{
			CurrentMenuWidget->AddToViewport();
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ATowerOffenseGameMode::StartGame, DelayTime, false);
}

void ATowerOffenseGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Start State"));
	RemoveCurrentWidget();
	TankPlayerController->SetPlayerEnabledState(true);
}

void ATowerOffenseGameMode::RestartGame()
{
	RemoveCurrentWidget();
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ATowerOffenseGameMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("End State"));
	if(IsValid(EndMenuWidgetClass))
	{
		CurrentMenuWidget = CreateWidget(GetGameInstance(), EndMenuWidgetClass);
		if(IsValid(CurrentMenuWidget))
		{
			CurrentMenuWidget->AddToViewport();
		}
	}
}

void ATowerOffenseGameMode::ShowMainMenu()
{
	if(IsValid(MainMenuWidgetClass))
	{
		CurrentMenuWidget = CreateWidget(GetGameInstance(), MainMenuWidgetClass);
		if (CurrentMenuWidget)
		{
			CurrentMenuWidget->AddToViewport();
		}
	}
}

void ATowerOffenseGameMode::RemoveCurrentWidget()
{
	if (CurrentMenuWidget)
	{
		CurrentMenuWidget->RemoveFromParent();
	}
}
