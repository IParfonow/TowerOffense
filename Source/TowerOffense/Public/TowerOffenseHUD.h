// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerOffenseHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATowerOffenseHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditDefaultsOnly, Category= "HUD")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UUserWidget> ShellsCounterClass;

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UUserWidget> EnemiesCountClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float VisibilityTimeLimit = 0.f;
	
	UPROPERTY(Transient)
	UUserWidget* HealthBar = nullptr;

	UPROPERTY(Transient)
	UUserWidget* ShellsCounter = nullptr;

	UPROPERTY(Transient)
	UUserWidget* EnemiesCounter = nullptr;
public:
	virtual void Tick(float DeltaSeconds) override;
	
	float VisibilityTime = 0.f;
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(bool bIsVisible);

	void InitializeHUDState();	
};
