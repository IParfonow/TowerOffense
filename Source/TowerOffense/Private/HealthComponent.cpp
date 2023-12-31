// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::PostInitProperties()
{
	Super::PostInitProperties();
	CurrentHealth = GetMaxHealth();
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor,float DamageAmount,const UDamageType* DamageType, AController* EventInstigator,
                                  AActor* DamageCauser)
{
	CurrentHealth = FMath::Max(0.f, CurrentHealth - DamageAmount);
	
	OnHealthChanged.Broadcast(CurrentHealth, -DamageAmount);
}

