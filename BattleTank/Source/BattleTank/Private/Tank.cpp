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

	CurrentRounds = StartingRounds;
	CurrentHealth = StartingHealth;

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
		CurrentRounds--;
		LastFireTime = FPlatformTime::Seconds();
	}
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// Prevent comparing floats to zero
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
	return DamageToApply;
}

int32 ATank::GetCurrentRounds() const
{
	return CurrentRounds;
}

float ATank::GetCurrentHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

EFiringState ATank::GetFiringState() const
{
	return FiringState;
}

EFiringState ATank::DetermineFiringState()
{
	if (CurrentRounds == 0)
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