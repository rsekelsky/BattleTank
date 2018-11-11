// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/AIController.h"
#include "CoreMinimal.h"
#include "TankAIController.generated.h"

// Forward declaration
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	// How close the AI tank must get to the player
	float AcceptanceRadius = 10;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	ATank* GetControlledTank() const;

	ATank* GetPlayerTank() const;
};
