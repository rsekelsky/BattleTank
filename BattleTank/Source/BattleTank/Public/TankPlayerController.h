// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// Forward declaration
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	ATank* GetControlledTank() const;

	// Get world location of linetrace through crosshair
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	// Returns an out parameter for aim direction, true if direction found
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	// Returns an out parameter for hit location, true if hits landscape
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	// Start the tank moving the barrel so that a shot would hit
	// where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Crosshair X location on screen
	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = 0.5;

	// Crosshair Y location on screen
	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.33333;

	// Linetrace range (max hit range)
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000;
};
