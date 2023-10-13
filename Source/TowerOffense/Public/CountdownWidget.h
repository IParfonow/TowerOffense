// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UCountdownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void StartCountdown(float TimeBetweenSteps);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	int32 CurrentCountdown = 0;
};
