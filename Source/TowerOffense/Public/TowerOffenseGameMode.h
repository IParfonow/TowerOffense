// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TowerOffenseHUD.h"
#include "GameFramework/GameModeBase.h"
#include "TowerOffenseGameMode.generated.h"

class ATankPawn;
class ATowerPawn;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	MainMenu,
	Loading,
	Playing,
	Paused,
	GameOver
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartCountDown, float, Time);

UCLASS()
class TOWEROFFENSE_API ATowerOffenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATowerOffenseGameMode();
	
	void AddPawnToArray(ATurretPawn* Pawn);
	
	UFUNCTION()
	void OnPawnDeath(ATurretPawn* DeadPawn);

	UFUNCTION()
	void ReducePrepareTime();

	UFUNCTION(BlueprintCallable)
	int32 GetEnemiesCount() const;

	UPROPERTY(BlueprintAssignable)
	FOnStartCountDown OnStartCountDown;

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void ChangeGameState(EGameState NewState);

	UFUNCTION(BlueprintCallable)
	void DecreaseLoadingControllers();
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void DelayStart();

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void StartGame();
	
	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void RestartGame();
	
	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void RemoveCurrentWidget();

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void ActivateWidget(TSubclassOf<UUserWidget>& Widget);
	

	UPROPERTY(BlueprintReadOnly, Category= "Game State")
	EGameState CurrenGameState = EGameState::MainMenu;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PrepareWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EndMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	ATowerOffenseHUD* HUD = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeBeforeStart = 5.0f;
	
	UPROPERTY()
	ATankPlayerController* TankPlayerController = nullptr;
	
	UPROPERTY()
	TArray<ATankPawn*> Players;

	UPROPERTY()
	TArray<ATowerPawn*> Enemies;

private:
	IAsyncLoadableObject* AssetLoader = nullptr;
	FTimerHandle RestartTimerHandle;

	int32 LoadingPlayerControllers = 0;

	UPROPERTY()
	UUserWidget* CurrentMenuWidget = nullptr;
};
