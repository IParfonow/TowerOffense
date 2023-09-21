// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"


#include "TowerOffenseGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ATurretPawn::HandleHealthChanges);
}


TArray<FString> ATurretPawn::GetBaseMeshMaterialSlots() const
{
	return TArray<FString> { TEXT("_Base_Material"), TEXT("Team_Material"), TEXT("Track_Material") }; 
}

void ATurretPawn::Fire()
{
	if(ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
	
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

		check(SpawnedProjectile)
		if(UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->GetProjectileMoveComponent())
		{
			SpawnedProjectile->SetOwner(this);
			ProjectileMovementComponent->InitialSpeed = ImpulseMagnitude;
		}
	}
}

void ATurretPawn::RotateTurretTowards(const FVector& TargetLocation)
{
	check(TurretMesh)
	const FVector TurretLocation = TurretMesh->GetComponentLocation();;

	FVector DirectionToCursor = TargetLocation - TurretLocation;
	DirectionToCursor.Z = 0.0f;
			
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	const FRotator TargetRotation = DirectionToCursor.Rotation();
			
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation , GetWorld()->GetDeltaSeconds(), TurretRotatingInterpSpeed);
	TurretMesh->SetWorldRotation(NewRotation);
}

void ATurretPawn::HandleHealthChanges(float NewHealth, float DamageAmount)
{
	if(NewHealth <= 0.f)
	{
		ATowerOffenseGameMode* GameMode = Cast<ATowerOffenseGameMode>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnPlayerDeath(this);
		}	
	}
	return;
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(BaseMesh && TurretMesh &&!MaterialParameterName.IsNone())
	{
		
		const int32 BaseMaterialIndex = BaseMesh->GetMaterialIndex(MaterialSlotName);
		const int32 TurretMaterialIndex = TurretMesh->GetMaterialIndex(MaterialSlotName);

		if(BaseMaterialIndex != INDEX_NONE && TurretMaterialIndex != INDEX_NONE)
		{
			if(UMaterialInstanceDynamic* TurretColorMaterial = BaseMesh->CreateDynamicMaterialInstance(TurretMaterialIndex))
			{
				TurretColorMaterial->SetVectorParameterValue(TeamColorParamName, TeamColor);
				TurretMesh->SetMaterial(BaseMaterialIndex, TurretColorMaterial);
			}
			if(UMaterialInstanceDynamic* HullColor = BaseMesh->CreateDynamicMaterialInstance(BaseMaterialIndex))
			{
				HullColor->SetVectorParameterValue(TeamColorParamName, TeamColor);
				BaseMesh->SetMaterial(BaseMaterialIndex, HullColor);
			}
		}
	}
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

