// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "TowerOffenseHUD.h"
#include "HealthComponentProvider.h"
#include "TowerOffenseHUD.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OwningPawn = GetOwningPlayerPawn();
	IHealthComponentProvider* HealthComponentProvider = Cast<IHealthComponentProvider>(OwningPawn);
	if(HealthComponentProvider)
	{
		UHealthComponent* HealthComponent = HealthComponentProvider->Execute_GetHealthComponent(OwningPawn);
		if(HealthComponent)
		{
			MaxHealth = HealthComponent->GetMaxHealth();
			Health = MaxHealth;

			HealthComponent->OnHealthChanged.AddDynamic(this, &UHealthBarWidget::SetCurrentHealth);
		}
	}
	
}

void UHealthBarWidget::SetCurrentHealth(float CurrentHealth, float DamageAmount)
{
	Health = CurrentHealth / MaxHealth;

	ATowerOffenseHUD* HUD = Cast<ATowerOffenseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	HUD->InitializeHUDState();
}
