// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include "TowerOffenseGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

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

	TowerOffenseGameMode = Cast<ATowerOffenseGameMode>(GetWorld()->GetAuthGameMode());
	if(!IsValid(TowerOffenseGameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("TO GameMode is not valid"));
	}
	PlayerController = Cast<ATankPlayerController>(this->GetController());
	if(!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("TO PlayerController is not valid"));
	}
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATurretPawn::HandleHealthChanges);
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(BaseMesh && TurretMesh &&!TeamColorParamName.IsNone())
	{
		const int32 BaseMaterialIndex = BaseMesh->GetMaterialIndex(MaterialSlotName);
		const int32 TurretMaterialIndex = TurretMesh->GetMaterialIndex(MaterialSlotName);

		if(BaseMaterialIndex != INDEX_NONE && TurretMaterialIndex != INDEX_NONE)
		{
			if(UMaterialInstanceDynamic* TurretColorMaterial = TurretMesh->CreateDynamicMaterialInstance(TurretMaterialIndex))
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

TArray<FString> ATurretPawn::GetBaseMeshMaterialSlots() const
{
	TArray<FString> Params;
	TArray<UMeshComponent*> MeshComponents;
	GetComponents(MeshComponents, true);
	
	for (const UMeshComponent* MeshComponent : MeshComponents)
	{
		for (const FName& Name : MeshComponent->GetMaterialSlotNames())
		{
			Params.AddUnique(Name.ToString());	
		}
	}
	return Params;
}

void ATurretPawn::Shoot()
{
	if(ProjectileClass)
	{
		ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation);

		check(SpawnedProjectile)
		if(UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->GetProjectileMoveComponent())
		{
			SpawnedProjectile->SetOwner(this);
			ProjectileMovementComponent->InitialSpeed = ImpulseMagnitude;

			LoadAsset(MuzzleFlashEffect.ToSoftObjectPath(), "MuzzleFlashEffect");
			LoadAsset(TurretShootSound.ToSoftObjectPath(), "TurretShootSound");
		}
	}
}

void ATurretPawn::LoadAsset(const FSoftObjectPath& AssetPath, const FString& Context)
{
	if(AssetPath.IsValid())
	{
		FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
		AssetLoader.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateUObject(this, &ATurretPawn::OnAssetLoaded, Context));
	}
}

void ATurretPawn::OnAssetLoaded(const FString Context)
{
	if(Context.Equals("MuzzleFlashEffect", ESearchCase::IgnoreCase))
	{
		UParticleSystem* MuzzleFlashEffectSystem = MuzzleFlashEffect.Get();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffectSystem, ProjectileSpawnLocation, ProjectileSpawnRotation);
	}
	else if(Context.Equals("TurretShootSound", ESearchCase::IgnoreCase))
	{
		USoundBase* TurretShootSoundBase = TurretShootSound.Get();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TurretShootSoundBase, ProjectileSpawnLocation, ProjectileSpawnRotation);
	}
	else if(Context.Equals("TurretExplosionSound", ESearchCase::IgnoreCase))
	{
		USoundBase* TurretShootSoundBase = TurretShootSound.Get();
		UGameplayStatics::SpawnSoundAtLocation(this, TurretShootSoundBase, GetActorLocation());	
	}
	else if(Context.Equals("TurretGetHitSound", ESearchCase::IgnoreCase))
	{
		USoundBase* TurretGetHitSoundBase = TurretGetHitSound.Get();
		UGameplayStatics::SpawnSoundAtLocation(this, TurretGetHitSoundBase, GetActorLocation());
	}
	else if(Context.Equals("ExplosionEffect", ESearchCase::IgnoreCase))
	{
		UParticleSystem* ExplosionEffectSystem = ExplosionEffect.Get();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffectSystem, GetActorLocation(), FRotator::ZeroRotator);
	}
	else if(Context.Equals("TurretExplosionSound", ESearchCase::IgnoreCase))
	{
		USoundBase* TurretExplosionSoundBase = TurretExplosionSound.Get();
		UGameplayStatics::SpawnSoundAtLocation(this, TurretExplosionSoundBase, GetActorLocation());
	}
}

void ATurretPawn::RegisterSpawnedPawn(ATurretPawn* SpawnedPawn)
{
	if(IsValid(TowerOffenseGameMode))
	{
		TowerOffenseGameMode->AddPawnToArray(SpawnedPawn);
	}
}

void ATurretPawn::RotateTurretTowards(const FVector& TargetLocation) const
{
	if(!TurretMesh)
	{
		return;
	}
	const FVector TurretLocation = TurretMesh->GetComponentLocation();;

	FVector DirectionToCursor = TargetLocation - TurretLocation;
	DirectionToCursor.Z = 0.0f;
			
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	const FRotator TargetRotation = DirectionToCursor.Rotation();
			
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation , GetWorld()->GetDeltaSeconds(), TurretRotatingInterpolationSpeed);
	TurretMesh->SetWorldRotation(NewRotation);
}

void ATurretPawn::HandleHealthChanges(const float NewHealth, float Delta)
{
	if(NewHealth <= 0.f)
	{
		LoadAsset(ExplosionEffect.ToSoftObjectPath(), "ExplosionEffect");
		LoadAsset(TurretExplosionSound.ToSoftObjectPath(), "TurretExplosionSound");

		TowerOffenseGameMode->OnPawnDeath(this);
		if(PlayerController)
		{
			PlayerController->ClientStartCameraShake(CameraShakeClass);
		}
	}
	else
	{
		LoadAsset(TurretGetHitSound.ToSoftObjectPath(), "TurretGetHitSound");
	}
}

UHealthComponent* ATurretPawn::GetHealthComponent_Implementation()
{
	return HealthComponent;
}
