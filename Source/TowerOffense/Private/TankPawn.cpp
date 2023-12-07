// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Components/InputComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TowerOffenseHUD.h"
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

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Engine Sound"));
	EngineSound->SetupAttachment(RootComponent);
	EngineSound->SetAutoActivate(false);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	RegisterSpawnedPawn(this);
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

	if(!bIsReloaded)
	{
		TimeSinceLastFire += DeltaSeconds;
		if(TimeSinceLastFire >= ReloadTime)
		{
			bIsReloaded = true;
		}
	}
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(InputToFire, ETriggerEvent::Triggered, this, &ATankPawn::Shoot);
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

	if (LeftDustEmitterComponent && LeftDustEmitterSpawnComponent)
	{
		LeftDustEmitterComponent->SetWorldLocation(LeftDustEmitterSpawnComponent->GetComponentLocation());
	}
	if (RightDustEmitterComponent && RightDustEmitterSpawnComponent)
	{
		RightDustEmitterComponent->SetWorldLocation(RightDustEmitterSpawnComponent->GetComponentLocation());
	}
}

void ATankPawn::SpawnEmitter()
{
	if(IsValid(TrackDust))
	{
		FVector LeftTrackLocation = LeftDustEmitterSpawnComponent->GetComponentLocation();
		FVector RightTrackLocation = RightDustEmitterSpawnComponent->GetComponentLocation();
	
		LeftDustEmitterComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, LeftTrackLocation, FRotator::ZeroRotator, FVector(1),false);
		RightDustEmitterComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrackDust, RightTrackLocation, FRotator::ZeroRotator, FVector(1),false);
	}
}

void ATankPawn::DestroyEmitter()
{
	if(LeftDustEmitterComponent && RightDustEmitterComponent)
	{
		LeftDustEmitterComponent->DestroyComponent();
		LeftDustEmitterComponent = nullptr;

		RightDustEmitterComponent->DestroyComponent();
		RightDustEmitterComponent=nullptr;
	}
}

void ATankPawn::LaunchTankEngine()
{
	if (EngineSound && EngineSound->IsRegistered())
	{
		EngineSound->Play();
	}
}

void ATankPawn::TurnRight(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	const FRotator TurnVector = FRotator(0.0f,  TankBaseTurnSpeed, 0.0f);

	const FRotator RotationSpeed = TurnVector * InputValue * DeltaSeconds;
	
	AddActorLocalRotation(RotationSpeed, true, nullptr, ETeleportType::TeleportPhysics);
}


void ATankPawn::Shoot()
{
	if(Ammo == 0)
	{
		return;
	}

	if(bIsReloaded)
	{
		Super::Shoot();
		Ammo--;
		TimeSinceLastFire = 0.f;
		if(CameraShakeClass)
		{
			PlayerController->ClientStartCameraShake(CameraShakeClass);
		}

		ATowerOffenseHUD* HUD = Cast<ATowerOffenseHUD>(UGameplayStatics::GetPlayerController(this,0)->GetHUD());
		if(HUD)
		{
			HUD->RestoreWidgetsVisibility();
		}
	}
	bIsReloaded = false;
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


