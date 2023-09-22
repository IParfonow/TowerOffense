// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerOffenseGameMode.h"
#include "TurretPawn.h"
#include "TankPawn.h"

ATowerOffenseGameMode::ATowerOffenseGameMode()
{
	DefaultPawnClass = ATankPawn::StaticClass();
}

void ATowerOffenseGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATowerOffenseGameMode::OnPlayerDeath(ATurretPawn* DeadPawn)
{
	DeadPawn->Destroy();
	UE_LOG(LogTemp, Warning, TEXT("You're literally dead."));
}
