
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, Category= "Health")
	float Health = 0.f;
	
	float MaxHealth = 0.f;

	UPROPERTY()
	APawn* OwningPawn = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth, float DamageAmount);
};
