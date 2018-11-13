// Copyright Rourke Sekelsky.

#include "Public/Tank.h"
#include "Public/TankAimingComponent.h"
#include "Public/Projectile.h"
#include "Engine/World.h"

ATank::ATank()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
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
	if (!ensure(TankAimingComponent))
	{
		return;
	}

	bool bIsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (bIsReloaded)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			TankAimingComponent->GetProjectileSocketLocation(),
			TankAimingComponent->GetProjectileSocketRotation()
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}
