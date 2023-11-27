// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"

#include "TowerHealthBarWidget.h"
#include "VectorTypes.h"
#include "Kismet/GameplayStatics.h"

ATowerPawn::ATowerPawn()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetRelativeLocation(FVector(0.f,0.f,0.f));
	SphereComponent->SetSphereRadius(1000);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerPawn::OnOverlapBegin); 
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATowerPawn::OnOverlapEnd);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar Component"));
	HealthBarComponent->SetupAttachment(RootComponent);
}

void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();
	RegisterSpawnedPawn(this);

	UTowerHealthBarWidget* TowerHealthBarWidget = Cast<UTowerHealthBarWidget>(HealthBarComponent->GetWidget());
	if(TowerHealthBarWidget)
	{
		TowerHealthBarWidget->SetUpHealthBarOwner(this);
	}
}

void ATowerPawn::Shoot()
{
	Super::Shoot();
}

void ATowerPawn::RegisterSpawnedPawn(ATurretPawn* SpawnedPawn)
{
	Super::RegisterSpawnedPawn(SpawnedPawn);
}

AActor* ATowerPawn::GetHighestPriorityTarget()
{
	const FVector SphereCenter = SphereComponent->GetComponentLocation();
	float MinDistance = TNumericLimits<float>::Max();
	AActor* ClosestTarget = nullptr;
	
	for(AActor* Target : OverlappingPawns)
	{
		if(!IsValid(Target))
		{
			continue;
		}
		
		FVector PlayerLocation = Target->GetActorLocation();
		const float DistanceToSphere = FVector::DistSquared(PlayerLocation, SphereCenter);
		
		if(DistanceToSphere < MinDistance && HasClearLineOfSightTo(Target))
		{
			MinDistance = DistanceToSphere;
			ClosestTarget = Target;
		}
	}
	return ClosestTarget;
}

void ATowerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for(TSubclassOf<APawn> TargetClass : TargetClasses)
	{
		if(OtherActor->IsA(TargetClass))
		{
			OverlappingPawns.Add(OtherActor);
			break;
		}
	}
}

void ATowerPawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingPawns.Remove(OtherActor);
}

bool ATowerPawn::HasClearLineOfSightTo(AActor* TargetActor) const
{
	if (!TargetActor)
	{
		return false;
	}

	FVector Start = TurretMesh->GetComponentLocation();
	FVector End = TargetActor->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); 

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	return bHit && (HitResult.GetActor() == TargetActor);
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(AActor* Target = GetHighestPriorityTarget())
	{
		RotateTurretTowards(Target->GetActorLocation());
		
		TimeSinceLastFire += DeltaSeconds;
			if(TimeSinceLastFire >= ReloadTime)
			{
				Shoot();
				TimeSinceLastFire = 0.f;
			}
	}
	else
	{
		TimeSinceLastFire = 0.f;
	}

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector HealthBarLocation = HealthBarComponent->GetComponentLocation();
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

	FVector PlayerCameraLocation = CameraLocation - HealthBarLocation;
	FRotator HealthBarRotation = PlayerCameraLocation.Rotation();
	HealthBarComponent->SetWorldRotation(HealthBarRotation);
}
