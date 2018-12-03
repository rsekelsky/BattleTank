// Copyright Rourke Sekelsky.

#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::DriveTrack(float Throttle)
{
	auto ForceApplied = Throttle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

void UTankTrack::SetThrottle(float Throttle)
{
	float ClampedThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(ClampedThrottle);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;

	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild)
		{
			continue;
		}
		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();

		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel)
		{
			continue;
		}
		ResultArray.Add(SprungWheel);
	}

	return ResultArray;
}
