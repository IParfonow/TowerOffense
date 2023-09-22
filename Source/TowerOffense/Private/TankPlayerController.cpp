// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

void ATankPlayerController::SetPlayerEnabledState(bool bIsEnabled)
{
	if(bIsEnabled)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}
