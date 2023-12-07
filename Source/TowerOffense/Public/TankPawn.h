// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "TankPawn.generated.h"

UCLASS()
class TOWEROFFENSE_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()
public:
	ATankPawn();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void AddMappingContextToInput() const;
	
	void LaunchTankEngine();
protected:
	virtual void BeginPlay() override;
	
	void MoveForward(const FInputActionValue& Value);
	void TurnRight(const FInputActionValue& Value);
	
	virtual void Shoot() override;

	void SpawnEmitter();
	void DestroyEmitter();

	
	
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn) override;

	//Components
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Tank")
	UCameraComponent* CameraComponent = nullptr;

	//Enhanced Input

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputMappingContext* InputMapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToFire = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToMoveForward = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToMoveRight = nullptr;

	//Movement
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float TankBaseSpeed = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float TankBaseTurnSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Ammunition")
	int32 Ammo = 0;
	
	bool bIsReloaded = false;

	//VFX
	
	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	UParticleSystem* TrackDust = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* LeftDustEmitterSpawnComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RightDustEmitterSpawnComponent = nullptr;

	UPROPERTY()
	UParticleSystemComponent* LeftDustEmitterComponent = nullptr;

	UPROPERTY()
	UParticleSystemComponent* RightDustEmitterComponent = nullptr;

	//SFX

	UPROPERTY(EditDefaultsOnly, Category= "SFX")
	UAudioComponent* EngineSound = nullptr;
	
};
