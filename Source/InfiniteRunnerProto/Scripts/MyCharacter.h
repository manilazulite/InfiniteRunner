// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class INFINITERUNNERPROTO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void MoveInputResponse(const FInputActionValue& Value);

	void HandleLaneChange(const FInputActionValue& Value);
	void UpdateLanePosition(float DeltaTime);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	int32 CurrentLane = 0;
	int32 MaxLane = 1;

	UPROPERTY(EditAnywhere, Category = "Lane System")
	float LaneOffset = 400.f;

	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Lane System")
	float SlideSpeed = 10.f;

	/*UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;*/
	

private:
	UPROPERTY(EditAnywhere, Category = "Moving Character")
	FVector CharMoveVelocity = FVector(100, 0, 0);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
