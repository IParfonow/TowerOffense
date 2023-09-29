// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditDefaultsOnly, Category= "Widgets")
	TSubclassOf<UUserWidget> LoseClassWidget;

	UPROPERTY(EditDefaultsOnly, Category= "Widgets")
	TSubclassOf<UUserWidget> WinClassWidget;
public:
	UFUNCTION(BlueprintCallable, Category= "Player")
	void SetPlayerEnabledState(bool bIsEnabled);

	UFUNCTION()
	void HandleEndGame(bool ArePlayersWin);
	

	UFUNCTION()
	void ShowEndGameWidget(TSubclassOf<UUserWidget> WidgetClass) const;
	
};
