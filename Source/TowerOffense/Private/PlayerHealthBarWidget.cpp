// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthBarWidget.h"

void UPlayerHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* PlayerPawn = GetOwningPlayerPawn();
	SetUpHealthBarOwner(PlayerPawn);

}
