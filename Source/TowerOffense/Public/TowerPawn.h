// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TowerPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENSE_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()
	ATowerPawn();
protected:
	virtual void BeginPlay() override;

	virtual void Shoot() override;
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn) override;

	AActor* GetHighestPriorityTarget();

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* HealthBarComponent = nullptr;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool HasClearLineOfSightTo(AActor* TargetActor) const;

private:

	UPROPERTY()
	TArray<AActor*> OverlappingPawns;

	UPROPERTY(EditAnywhere, Category= "Targeting")
	TArray<TSubclassOf<APawn>> TargetClasses;
	 
	


public:
	virtual void Tick(float DeltaSeconds) override;
};
