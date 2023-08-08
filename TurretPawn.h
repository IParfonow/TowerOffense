// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class TOWEROFFENSE_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower", meta=(GetOptions = "GetBaseMeshMaterialSlots"))
	FName MaterialSlotName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	FName MaterialParameterName;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	FLinearColor TeamColor = FLinearColor::White;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	UStaticMeshComponent* TurretMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	FName TeamColorParamName = NAME_None;
	
	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlots() const;
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
