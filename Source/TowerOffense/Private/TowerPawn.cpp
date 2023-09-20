// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"

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
}

void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATowerPawn::Fire() const
{
	UE_LOG(LogTemp, Warning, TEXT("Tower Shoots!"));
}

AActor* ATowerPawn::GetHighestPriorityTarget()
{
	FVector SphereCenter = SphereComponent->GetComponentLocation();
	float MinDistance = TNumericLimits<float>::Max();
	AActor* ClosestTarget = nullptr;
	
	for(AActor* Target : OverlappingPawns)
	{
		if(!IsValid(Target))
		{
			continue;
		}
		
		FVector PlayerLocation = Target->GetActorLocation();
		float DistanceToSphere = FVector::DistSquared(PlayerLocation, SphereCenter);
		
		if(DistanceToSphere < MinDistance)
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

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(AActor* Target = GetHighestPriorityTarget())
	{
		RotateTurretTowards(Target->GetActorLocation());
		
		TimeSinceLastFire += DeltaSeconds;
			if(TimeSinceLastFire >= FireInterval)
			{
				Fire();
				TimeSinceLastFire = 0.f;
			}
	}
}