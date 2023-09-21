// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, DamageAmount);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFFENSE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;

	float CurrentHealth = 0.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor,float DamageAmount,const UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser); 
};
