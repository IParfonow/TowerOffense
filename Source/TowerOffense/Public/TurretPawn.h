// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "HealthComponent.h"
#include "Projectile.h"
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
	
	
	UFUNCTION()
	TArray<FString> GetBaseMeshMaterialSlots() const;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	void RotateTurretTowards(const FVector& TargetLocation);

	UFUNCTION()
	void HandleHealthChanges(float NewHealth, float Delta);
public:

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
