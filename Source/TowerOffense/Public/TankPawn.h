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
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//FUNTCTIONS
	
	void AddMappingContextToInput() const;

protected:
	
	void MoveForward(const FInputActionValue& Value);
	void SpawnEmitter();
	void DestroyEmitter();
	void TurnRight(const FInputActionValue& Value);
	virtual void Fire() override;
	virtual void RegisterSpawnedPawn(ATurretPawn* SpawnedPawn) override;

	//PROPERTIES
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Tank")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputMappingContext* InputMapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToFire = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToMoveForward = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enhanced Input")
	UInputAction* InputToMoveRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float TankBaseSpeed = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float TankBaseTurnSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Ammunition")
	int32 Ammo = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Tank")
	float FireInterval = 3.f;

	bool bIsReloaded = false;
public:

	//VFX
protected:
	
	UPROPERTY(EditDefaultsOnly, Category= "Effects")
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

	UPROPERTY(EditDefaultsOnly, Category= "Audio")
	UAudioComponent* EngineSound = nullptr;
	
};
