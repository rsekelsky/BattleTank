// Copyright Rourke Sekelsky.

#include "Public/TankAIController.h"
#include "Public/Tank.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
// Depends on TankMovementComponent through overridden MoveToActor method

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ensure(GetPlayerTank()))
	{
		// Move towards the player
		MoveToActor(GetPlayerTank(), AcceptanceRadius);

		// Aim/fire at player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
		GetControlledTank()->Fire();
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
