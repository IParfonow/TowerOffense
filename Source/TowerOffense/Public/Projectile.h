// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TOWEROFFENSE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UProjectileMovementComponent* GetProjectileMoveComponent() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category= "Projectile")
	UStaticMeshComponent* ProjectileMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category= "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Effects")
	UParticleSystem* HitEffect = nullptr;
	
	UPROPERTY(EditAnywhere, Category= "Projectile")
	float Damage = 0.f;

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
