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
	FName MaterialParameterName = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	FLinearColor TeamColor = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	UStaticMeshComponent* TurretMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	FName TeamColorParamName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category= "Turret Tower")
	float TurretRotatingInterpSpeed = 10.f;
	
	
	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlots() const;

	UFUNCTION()
	void RotateTurretTowards(const FVector& MouseCursor);
public:

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
