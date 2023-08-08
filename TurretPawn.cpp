// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
}


TArray<FString> ATurretPawn::GetBaseMeshMaterialSlots() const
{
	return TArray<FString> { TEXT("_Base_Material"), TEXT("Team_Material"), TEXT("Track_Material") }; 
}

// Called every frame
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

