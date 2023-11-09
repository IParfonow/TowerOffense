// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthBarWidget.h"
#include "TowerHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API UTowerHealthBarWidget : public UHealthBarWidget
{
	GENERATED_BODY()

protected:

	void virtual NativeConstruct() override;
	
};
