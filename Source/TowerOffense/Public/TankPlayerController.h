// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "AsyncLoadableObject.h"
#include "TankPlayerController.generated.h"

class ATurretPawn;

UCLASS()
class TOWEROFFENSE_API ATankPlayerController : public APlayerController, public IAsyncLoadableObject
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category= "Widgets")
	TSubclassOf<UUserWidget> LoseClassWidget;

	UPROPERTY(EditDefaultsOnly, Category= "Widgets")
	TSubclassOf<UUserWidget> WinClassWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Game Logic")
	float EndWidgetTimeDuration = 5.0f;
public:

	float GetEndWidgetTimeDuration() const;
	
	UFUNCTION(BlueprintCallable, Category= "Player")
	void SetPlayerEnabledState(bool bIsEnabled);

	UFUNCTION()
	void HandleEndGame(bool ArePlayersWin);
	
	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void ShowEndGameWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category= "Game Logic")
	void HideEndGameWidget();

	virtual void LoadAssets() override;

	virtual void OnAssetsLoaded() override;

	virtual FOnAssetsLoaded& GetOnAssetsLoadedEvent() override;
	
	FOnAssetsLoaded DecreaseControllersCounter;
private:
	FTimerHandle TimerHandle;
	
	ATurretPawn* ControlledPawn = nullptr;
	
	UPROPERTY()
	UUserWidget* EndGameWidget = nullptr;
};
