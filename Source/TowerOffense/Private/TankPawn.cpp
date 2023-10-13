// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Components/InputComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"


ATankPawn::ATankPawn()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	SpringArmComponent->SetRelativeRotation(FRotator(0.0f,-20.0f,0.0f));
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,100.0f),FRotator(-30.0f,0.0f,0.0f));
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);	

	LeftDustEmitterSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Left Dust Emitter Scene Comp"));
	LeftDustEmitterSpawnComponent->SetupAttachment(BaseMesh);

	RightDustEmitterSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Right Dust Emitter Scene Comp"));
	RightDustEmitterSpawnComponent->SetupAttachment(BaseMesh);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
}
	
void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(IsValid(PlayerController))
	{
		FHitResult HitResult;		
		const bool bHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false , HitResult);
		if(bHit)
		{
			RotateTurretTowards(HitResult.Location);	
			DrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 12, FColor::Red, false, -1, 0, 1);
		}
	}
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(InputToFire, ETriggerEvent::Triggered, this, &ATankPawn::Fire);
	EnhancedInputComponent->BindAction(InputToMoveForward, ETriggerEvent::Triggered, this, &ATankPawn::MoveForward);
	EnhancedInputComponent->BindAction(InputToMoveRight, ETriggerEvent::Triggered, this, &ATankPawn::TurnRight);
	
	EnhancedInputComponent->BindAction(InputToMoveForward, ETriggerEvent::Started, this, &ATankPawn::SpawnEmitter);
	EnhancedInputComponent->BindAction(InputToMoveForward, ETriggerEvent::Completed, this, &ATankPawn::DestroyEmitter);
}


void ATankPawn::MoveForward(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	const FVector MoveVector = FVector(TankBaseSpeed, 0.f, 0.f);
	const FVector MoveSpeed = MoveVector * InputValue;

	AddActorLocalOffset(MoveSpeed, true);
}

void ATankPawn::SpawnEmitter()
{
	check(TrackDust);
	
	FVector LeftTrackLocation = LeftDustEmitterSpawnComponent->GetComponentLocation();
	FVector RightTrackLocation = RightDustEmitterSpawnComponent->GetComponentLocation();
	
	LeftDustEmitterComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, LeftTrackLocation);
	RightDustEmitterComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, RightTrackLocation);
}

void ATankPawn::DestroyEmitter()
{
	check(LeftDustEmitterComponent);
	check(RightDustEmitterComponent);

	LeftDustEmitterComponent->DestroyComponent();
	LeftDustEmitterComponent = nullptr;

	RightDustEmitterComponent->DestroyComponent();
	RightDustEmitterComponent=nullptr;

}


void ATankPawn::TurnRight(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	const FRotator TurnVector = FRotator(0.0f,  TankBaseTurnSpeed, 0.0f);

	const FRotator RotationSpeed = TurnVector * InputValue * DeltaSeconds;
	
	AddActorLocalRotation(RotationSpeed, true, 0, ETeleportType::TeleportPhysics);
}


void ATankPawn::Fire()
{
	Super::Fire();
	RegisterSpawnedPawn(this);
}

void ATankPawn::RegisterSpawnedPawn(ATurretPawn* SpawnedPawn)
{
	Super::RegisterSpawnedPawn(SpawnedPawn);
}

void ATankPawn::AddMappingContextToInput() const
{
	if (!InputMapping)
	{
		return;
	}
	const APlayerController* TankPlayerController = Cast<APlayerController>(this->GetController());
	if (!TankPlayerController)
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer());
	if(!LocalPlayer)
	{
		return;
	}
	
	auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem)
	{
		return;
	}
	
	InputSystem->AddMappingContext(InputMapping, 0);
}

