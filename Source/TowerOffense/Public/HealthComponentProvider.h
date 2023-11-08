// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthComponentProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthComponentProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWEROFFENSE_API IHealthComponentProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "HealthComponentProvider")
	UHealthComponent* GetHealthComponent();
};
