// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Components/InputComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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

}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	const APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer());
	
	if (PlayerController)
    {
		if(LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (InputMapping)
				{
					InputSystem->AddMappingContext(InputMapping, 0);
				}
			}
		}
    }
}
	
void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(PlayerController)
	{
		const bool bHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false , HitResult);
		if(bHit)
		{
			RotateTurretTowards(HitResult);	
			DrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 12, FColor::Red, false, -1, 0, 1);
		}
	}
	
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(InputToFire, ETriggerEvent::Triggered, this, &ATankPawn::TriggerFire);
	EnhancedInputComponent->BindAction(InputToMoveForward, ETriggerEvent::Triggered, this, &ATankPawn::MoveForward);
	EnhancedInputComponent->BindAction(InputToMoveRight, ETriggerEvent::Triggered, this, &ATankPawn::MoveRight);
	
}


void ATankPawn::MoveForward(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		const FVector MoveVector = FVector(TankBaseSpeed,0.f,0.f);
		AddActorLocalOffset(MoveVector * Value.Get<float>(), true,nullptr,ETeleportType::None);
	}
	
}


void ATankPawn::MoveRight(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		const FRotator TurnVector = FRotator(0.0f,TankBaseTurnSpeed,0.0f);
		AddActorLocalRotation(TurnVector * Value.Get<float>() * DeltaSeconds, true, nullptr, ETeleportType::None);
	}
}


void ATankPawn::TriggerFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot!"));
}


