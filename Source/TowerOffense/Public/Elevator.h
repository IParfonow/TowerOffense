// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "Elevator.generated.h"

UENUM(BlueprintType) 
enum Easing : uint8
{
	Linear = 0,	
	Circ = 1,
	EaseInOut = 2,
	ExpoInOut = 3
};


UCLASS()
class TOWEROFFENSE_API ATowerElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UStaticMeshComponent* TowerElevator = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UBoxComponent* BoxComponent = nullptr;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Movement")
	float Speed = 100;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Movement")
	float DistanceToMove = 2050;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Movement")
	TEnumAsByte<Easing> WayToInterpolate = Linear;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Movement")
	FVector MovementDirection = FVector(0,0,1);

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float StopTime = 0.f;

	//Functions

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	FVector StartPosition;
	FVector EndPosition;

	float StopDuration = 0.f;
	float MoveDuration;
	float Alpha = 0.0f;
	bool bIsMovingForward = true;
	bool bIsLifting = false;

	UPROPERTY()
	ATankPawn* Player = nullptr;

	FVector GetInterpolation(const FVector& A, const FVector& B, const float& InAlpha, const float& Exp) const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
