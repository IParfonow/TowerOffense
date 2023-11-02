// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "HealthComponent.h"
#include "Projectile.h"
#include "TowerOffenseGameMode.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class TOWEROFFENSE_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPawn();

	//PROPERTIES
protected:
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret", meta=(GetOptions = "GetBaseMeshMaterialSlots"))
	FName MaterialSlotName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	FName MaterialParameterName = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	FLinearColor TeamColor = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	UStaticMeshComponent* TurretMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	FName TeamColorParamName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret")
	float TurretRotatingInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category= "Turret")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Turret")
	float ImpulseMagnitude = 7000.f;

	UPROPERTY(EditDefaultsOnly, Category= "Health")
	UHealthComponent* HealthComponent = nullptr;

	//EFFECTS

	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	UParticleSystem* ExplosionEffect = nullptr;

	UPROPERTY()
	ATowerOffenseGameMode* TowerOffenseGameMode = nullptr;

	//FUNCTIONS
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlots() const;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn);

	UFUNCTION()
	void RotateTurretTowards(const FVector& TargetLocation);

	UFUNCTION()
	void HandleHealthChanges(float NewHealth, float Delta);

	//SFX
	
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretGetHitSoundBase = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretShootSoundBase = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	USoundBase* TurretExplosionSoundBase = nullptr;
	
public:

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
