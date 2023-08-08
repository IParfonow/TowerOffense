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
	
	if(BaseMesh && !MaterialParameterName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Material slot name: %s"), *MaterialSlotName.ToString());
		
		const int32 MaterialIndex = BaseMesh->GetMaterialIndex(MaterialSlotName);
		UE_LOG(LogTemp, Warning, TEXT("Material index: %d"), MaterialIndex);

		if(MaterialIndex != INDEX_NONE)
		{
			if(UMaterialInstanceDynamic* HullColor = UMaterialInstanceDynamic::Create(BaseMesh->GetMaterial(MaterialIndex), this))
			{
				HullColor->SetVectorParameterValue(FName("TeamColor"), SlotColor);
				BaseMesh->SetMaterial(MaterialIndex, HullColor);
			}
		}
	}
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

