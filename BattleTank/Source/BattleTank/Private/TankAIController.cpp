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

void ATankAIController::SetPawn(APawn* Pawn)
{
	Super::SetPawn(Pawn);

	if (!Pawn)
	{
		return;
	}
	auto ControlledTank = Cast<ATank>(Pawn);
	ControlledTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnControlledTankDeath);
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
		// Move towards the player
		MoveToActor(GetPlayerTank(), AcceptanceRadius);

		// Aim at player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

		// Fire if locked
		if (GetControlledTank()->GetFiringState() == EFiringState::Locked)
		{
			GetControlledTank()->Fire();
		}
	}
}

void ATankAIController::OnControlledTankDeath()
{
	GetControlledTank()->DetachFromControllerPendingDestroy();
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
