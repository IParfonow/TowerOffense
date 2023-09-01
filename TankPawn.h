// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "Components/InterpToMovementComponent.h"
#include "TankPawn.generated.h"

/**
 * 
 */
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

protected:

	void MoveForward(const FInputActionValue& Value);
	void TurnRight(const FInputActionValue& Value);
	void TriggerFire();

	void AddMappingContextToInput() const;	
	
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
	float TankBaseSpeed = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float TankBaseTurnSpeed = 20.0f;
};
