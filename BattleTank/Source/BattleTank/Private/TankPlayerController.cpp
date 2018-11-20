// Copyright Rourke Sekelsky.

#include "Public/TankPlayerController.h"
#include "Public/Tank.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ensure(GetControlledTank()))
	{
		return;
	}
	FoundTank(GetControlledTank());
}

void ATankPlayerController::SetPawn(APawn* Pawn)
{
	Super::SetPawn(Pawn);

	if (!Pawn)
	{
		return;
	}
	auto ControlledTank = Cast<ATank>(Pawn);
	ControlledTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnControlledTankDeath);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank())
	{
		return;
	}

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

void ATankPlayerController::OnControlledTankDeath()
{
	StartSpectatingOnly();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

FVector2D ATankPlayerController::GetCrosshairScreenLocation() const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	return FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// De-project the screen position of crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(GetCrosshairScreenLocation(), LookDirection))
	{
		// Linetrace along LookDirection and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y, 
		CameraWorldLocation, 
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	// Debug line trace
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = TraceTag;
	
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	bool bHaveLineTrace = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera
		//,CollisionParams // Uncomment this for line trace
	);
	if (bHaveLineTrace)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	
	HitLocation = FVector(0);
	return false; // Line trace failed
}
