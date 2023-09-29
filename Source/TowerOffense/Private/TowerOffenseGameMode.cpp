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
				GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle,  this, &ATowerOffenseGameMode::RestartGame, TimeBeforeRestart ,  false);
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
				GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle,  this, &ATowerOffenseGameMode::RestartGame, TimeBeforeRestart ,  false);
			}
		}
	}
	DeadPawn->Destroy();
}

void ATowerOffenseGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
