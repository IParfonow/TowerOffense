// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Component"));
	ProjectileMeshComponent->SetSimulatePhysics(true);	
	RootComponent = ProjectileMeshComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

float AProjectile::GetImpulseMagnitude() const
{
	return ProjectileMovementComponent->InitialSpeed;
}

UStaticMeshComponent* AProjectile::GetMeshComponent() const
{
	return ProjectileMeshComponent;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

