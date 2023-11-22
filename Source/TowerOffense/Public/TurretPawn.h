// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/NavMovementComponent.h"
#include "HealthComponent.h"
#include "HealthComponentProvider.h"
#include "Projectile.h"
#include "TowerOffenseGameMode.h"
#include "TurretPawn.generated.h"


class ATankPlayerController;

UCLASS()
class TOWEROFFENSE_API ATurretPawn : public APawn, public IHealthComponentProvider
{
	GENERATED_BODY()

public:
	ATurretPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	//Components
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UCapsuleComponent* CapsuleComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UStaticMeshComponent* BaseMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UStaticMeshComponent* TurretMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Health")
	UHealthComponent* HealthComponent = nullptr;


	//Turret Properties
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret", meta=(GetOptions = "GetBaseMeshMaterialSlots")) //BP Parameter Choice of Materials
	FName MaterialSlotName = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret") //Name of parameter in needed element
	FName TeamColorParamName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	FLinearColor TeamColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, Category= "Turret")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Turret")
	float ImpulseMagnitude = 7000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Turret")
	float ReloadTime = 3.f;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	float TurretRotatingInterpolationSpeed = 10.f;

	
	//VFX
	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	UParticleSystem* ExplosionEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	
	//SFX
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretGetHitSoundBase = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretShootSoundBase = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretExplosionSoundBase = nullptr;

	
	//GamePlay
	float TimeSinceLastFire = 0.0f;
	
	UPROPERTY()
	ATowerOffenseGameMode* TowerOffenseGameMode = nullptr;

	UPROPERTY()
	ATankPlayerController* PlayerController = nullptr;

	//FUNCTIONS
	virtual void BeginPlay() override;
	
	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlots() const;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn);

	UFUNCTION()
	void RotateTurretTowards(const FVector& TargetLocation) const;

	UFUNCTION()
	void HandleHealthChanges(const float NewHealth, float Delta);

	virtual UHealthComponent* GetHealthComponent_Implementation() override;
};
