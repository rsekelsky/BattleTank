// Copyright Rourke Sekelsky.

#include "Public/TankAimingComponent.h"
#include "Public/TankBarrel.h"
#include "Public/TankTurret.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!ensure(Barrel))
	{
		return;
	}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	// Calculate OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret))
	{
		return;
	}

	// Calculate difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetComponentRotation();
	auto AimDirectionRotator = AimDirection.Rotation();
	auto DeltaRotator = AimDirectionRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.GetNormalized().Pitch);
	Turret->Rotate(DeltaRotator.GetNormalized().Yaw);
}

FVector UTankAimingComponent::GetProjectileSocketLocation() const
{
	if (!ensure(Barrel))
	{
		return FVector(0);
	}
	return Barrel->GetSocketLocation(FName("Projectile"));
}

FRotator UTankAimingComponent::GetProjectileSocketRotation() const
{
	if (!ensure(Barrel))
	{
		return FRotator(0);
	}
	return Barrel->GetSocketRotation(FName("Projectile"));
}

bool UTankAimingComponent::IsBarrelMoving() const
{
	if (!ensure(Barrel))
	{
		return false;
	}
	
	// Calculate difference between current barrel orientation and AimDirection
	auto BarrelForward = Barrel->GetForwardVector();
	auto DeltaVector = BarrelForward - AimDirection;
	return !DeltaVector.IsNearlyZero(0.001);
}
