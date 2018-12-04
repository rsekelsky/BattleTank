// Copyright Rourke Sekelsky.

#pragma once

#include "Classes/AIController.h"
#include "CoreMinimal.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * Responsible for controlling enemy tanks in the world.
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// How close the AI tank must get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 12000;

private:
	virtual void BeginPlay() override;

	// To subscribe to ATank->OnDeath broadcast
	virtual void SetPawn(APawn* Pawn) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnControlledTankDeath();

	ATank* GetControlledTank() const;

	ATank* GetPlayerTank() const;
};
