// Fill out your copyright notice in the Description page of Project Settings.


#include "Scripts/MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"


#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/Input/IMC_Runner"));
	if (IMC.Succeeded()) DefaultMappingContext = IMC.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(TEXT("/Game/Input/IA_MoveLeftRight"));
	if (MoveActionAsset.Succeeded()) MoveAction = MoveActionAsset.Object;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation(); // Initialize target

	//Get Player Controller
	if (APlayerController * pc = Cast<APlayerController>(GetController()))
	{
		//Get Local Player
		if (ULocalPlayer* localPlayer = pc->GetLocalPlayer())
		{
			//Get Enhanced Input Player Subsystems(Local)
			if (UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
			{
				enhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}	

	//ensure(enhancedInputSubsystem);

}

void AMyCharacter::HandleLaneChange(const FInputActionValue& Value)
{
	int Direction = Value.Get<float>() > 0 ? 1 : -1;
	int32 DesiredLane = FMath::Clamp(CurrentLane + Direction, -MaxLane, MaxLane);

	if (DesiredLane != CurrentLane)
	{
		CurrentLane = DesiredLane;

		TargetLocation = GetActorLocation();
		TargetLocation.Y = CurrentLane * LaneOffset; // Y-axis lanes: -400, 0, +400
	}
}

void AMyCharacter::UpdateLanePosition(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, SlideSpeed);
	SetActorLocation(NewLocation);
}

//void AMyCharacter::MoveInputResponse(const FInputActionValue& Value)
//{
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//
//	}
//}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLanePosition(DeltaTime);

	//Move the player
	FVector currentLocation = GetActorLocation();

	//Add Velocity to the Current Location
	currentLocation += CharMoveVelocity * DeltaTime;

	//Set Actor Location
	SetActorLocation(currentLocation);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (auto* enhancedinputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedinputcomponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLaneChange);

		//enhancedinputcomponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//enhancedinputcomponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

