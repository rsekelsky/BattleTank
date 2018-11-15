// Copyright Rourke Sekelsky.

#include "Public/Tank.h"
#include "Public/TankAimingComponent.h"
#include "Public/Projectile.h"
#include "Engine/World.h"

ATank::ATank()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();

	// Allow first fire after initial reload
	LastFireTime = FPlatformTime::Seconds();
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FiringState = DetermineFiringState();
}

void ATank::AimAt(FVector HitLocation)
{
	if (!ensure(TankAimingComponent))
	{
		return;
	}
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		if (!ensure(TankAimingComponent && ProjectileBlueprint))
		{
			return;
		}

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			TankAimingComponent->GetProjectileSocketLocation(),
			TankAimingComponent->GetProjectileSocketRotation()
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		Rounds--;
		LastFireTime = FPlatformTime::Seconds();
	}
}

int32 ATank::GetRoundsLeft() const
{
	return Rounds;
}

EFiringState ATank::GetFiringState() const
{
	return FiringState;
}

EFiringState ATank::DetermineFiringState()
{
	if (Rounds == 0)
	{
		return EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		return EFiringState::Reloading;
	}
	else if (!ensure(TankAimingComponent))
	{
		return EFiringState::Reloading;
	}
	else if (TankAimingComponent->IsBarrelMoving())
	{
		return EFiringState::Aiming;
	}
	else
	{
		return EFiringState::Locked;
	}
}