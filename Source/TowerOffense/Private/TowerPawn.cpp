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
	SphereComponent->SetCollisionProfileName("Trigger");

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

TArray<float> ATowerPawn::GetPlayersDistanceToSphere(const TArray<AActor*>& Players) const
{
	FVector SphereCenter = SphereComponent->GetComponentLocation();
	TArray<float> Distances;
	for(AActor* Player : Players)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		float DistanceToSphere = FVector::Dist(PlayerLocation, SphereCenter);
		Distances.Add(DistanceToSphere);
	}
	return Distances;
}

FVector ATowerPawn::GetTheClosestPlayerLocation() const
{
	int32 ClosestPlayerIndex = INDEX_NONE;
	float MinDistance = TNumericLimits<float>::Max();
	TArray<float> Distances = GetPlayersDistanceToSphere(OverlappingPawns);

	for(int32 i = 0; i < Distances.Num(); ++i)
	{
		if(Distances[i] < MinDistance)
		{
			MinDistance = Distances[i];
			ClosestPlayerIndex = i;
		}
	}
	if(ClosestPlayerIndex != INDEX_NONE)
	{
		return OverlappingPawns[ClosestPlayerIndex]->GetActorLocation();
	}
	return FVector(0.f,0.f,0.f);
}

void ATowerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		ATankPawn* PawnActor = Cast<ATankPawn>(OtherActor);
		if(PawnActor)
		{
			OverlappingPawns.Add(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("overlap"));
		}
	}
	
}

void ATowerPawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap ended"));
	OverlappingPawns.Remove(OtherActor);
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!OverlappingPawns.IsEmpty())
	{
		RotateTurretTowards(GetTheClosestPlayerLocation());
		TimeSinceLastFire += DeltaSeconds;
			if(TimeSinceLastFire >= FireInterval)
			{
				Fire();
				TimeSinceLastFire = 0.f;
			}
	}
}