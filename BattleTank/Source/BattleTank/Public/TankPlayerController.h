// Copyright Rourke Sekelsky.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATank;

/**
 * Responsible for aiming the tank turret and barrel.
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank* GetControlledTank() const;

	// TODO Determine if we can remove this function somehow
	// Blueprint event to set tank reference for player UI
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundTank(ATank* Tank);

private:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	// Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Get screen coordinates of crosshair
	FVector2D GetCrosshairScreenLocation() const;

	// TODO Redo HitLocation structure so aiming above max range behaves the same as aiming below min range
	// Get world location of linetrace through crosshair
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	// Returns an out parameter for aim direction, true if direction found
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	// Returns an out parameter for hit location, true if hits landscape
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

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
