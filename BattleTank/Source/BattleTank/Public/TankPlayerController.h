// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Math/Vector.h"
#include "UObject/UnrealType.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;

private:
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
	float CrosshairXLocation = 0.5f;

	// Crosshair Y location on screen
	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.33333f;

	// Linetrace range (max hit range)
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.f;
};
