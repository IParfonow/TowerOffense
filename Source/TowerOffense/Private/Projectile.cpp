// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Component"));
	ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMeshComponent->SetSimulatePhysics(true);

	
	RootComponent = ProjectileMeshComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

float AProjectile::GetImpulsePower() const
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
	this->Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

