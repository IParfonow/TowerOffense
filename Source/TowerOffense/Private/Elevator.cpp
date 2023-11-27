// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"


// Sets default values
ATowerElevator::ATowerElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerElevator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Elevator"));
	RootComponent = TowerElevator;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerElevator::OnOverlapBegin); 
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATowerElevator::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATowerElevator::BeginPlay()
{
	Super::BeginPlay();
	if(TowerElevator)
	{
		StartPosition = TowerElevator->GetComponentLocation();
		EndPosition = StartPosition + MovementDirection * DistanceToMove;
		MoveDuration = DistanceToMove / Speed;
	}
}

void ATowerElevator::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ATankPawn>(OtherActor);
	if(IsValid(Player))
	{
		const FVector PlatformLocation = TowerElevator->GetComponentLocation();
		Player->SetActorLocation(FVector(PlatformLocation.X, PlatformLocation.Y, PlatformLocation.Z + 120));
		bIsLifting = true;
	}
}

void ATowerElevator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player  = nullptr;
}


FVector ATowerElevator::GetInterpolation(const FVector& A, const FVector& B, const float& InAlpha, const float& Exp) const
{
	switch (WayToInterpolate.GetValue())
	{
	case EaseInOut:
		{
			return FMath::InterpEaseInOut(A, B, InAlpha, Exp);
		}
	case Linear:
		{
			return FMath::Lerp(A, B, InAlpha); 
		}
	case Circ:
		{
			return FMath::InterpCircularIn(A,B,InAlpha);
		}
	case ExpoInOut:
		{
			return FMath::InterpExpoInOut(A,B,InAlpha);
		}
	default:
		{
			return FVector::ZeroVector;
		}
	}
}

// Called every frame
void ATowerElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	bIsLifting = true;
	if(bIsMovingForward)
	{
		Alpha += DeltaTime / MoveDuration;
	}
	else
	{
		Alpha -= DeltaTime / MoveDuration;
	}

	Alpha = FMath::Clamp(Alpha, 0.0f,1.0f);
	

	if((Alpha >= 1.0f && bIsMovingForward) || (Alpha <= 0.0f && !bIsMovingForward))
	{
		bIsLifting = false;
		StopDuration+= DeltaTime;
		if(StopDuration >= StopTime)
		{
			bIsMovingForward = !bIsMovingForward;
			StopDuration = 0.f;
		}
	}
	TowerElevator->SetWorldLocation(GetInterpolation(StartPosition,EndPosition, Alpha, 2.0f));
	if(IsValid(Player) && bIsLifting)
	{
		const FVector PlatformLocation = TowerElevator->GetComponentLocation();
		Player->SetActorLocation(FVector(PlatformLocation.X, PlatformLocation.Y, PlatformLocation.Z + 120));
	}
}

