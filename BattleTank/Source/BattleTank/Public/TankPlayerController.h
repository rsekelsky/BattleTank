// Copyright Rourke Sekelsky.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// Forward declaration
class ATank;
class UTankAimingComponent;

/**
 * Responsible for helping the player aim.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank* GetControlledTank() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponentReference);

private:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	// Get world location of linetrace through crosshair
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	// Returns an out parameter for aim direction, true if direction found
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	// Returns an out parameter for hit location, true if hits landscape
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	// Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Crosshair X location on screen
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5;

	// Crosshair Y location on screen
	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333;

	// Linetrace range (max hit range)
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;
};
