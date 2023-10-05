// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "TowerOffenseGameMode.generated.h"

class ATankPawn;
class ATowerPawn;

UCLASS()
class TOWEROFFENSE_API ATowerOffenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATowerOffenseGameMode();
	
	void AddPawnToArray(ATurretPawn* Pawn);
	
	UFUNCTION()
	void OnPawnDeath(ATurretPawn* DeadPawn);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void RestartGame();

	UPROPERTY(EditDefaultsOnly)
	float TimeBeforeRestart = 5.0f;
	
	UPROPERTY()
	ATankPlayerController* TankPlayerController = nullptr;

	UPROPERTY()
	TArray<ATankPawn*> Players;

	UPROPERTY()
	TArray<ATowerPawn*> Enemies;
private:
	FTimerHandle RestartTimerHandle;
};
