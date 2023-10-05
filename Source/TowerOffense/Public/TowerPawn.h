// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "TurretPawn.h"
#include "Components/SphereComponent.h"
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

	virtual void Fire() override;
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn) override;

	AActor* GetHighestPriorityTarget();

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Tower")
	float FireInterval = 3.f;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY()
	TArray<AActor*> OverlappingPawns;

	UPROPERTY(EditAnywhere, Category= "Targeting")
	TArray<TSubclassOf<APawn>> TargetClasses;
	 
	float TimeSinceLastFire = 0.0f;


public:
	virtual void Tick(float DeltaSeconds) override;
};
