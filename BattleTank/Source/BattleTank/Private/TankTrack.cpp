// Copyright Rourke Sekelsky.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	// TODO clamp throttle value
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation, FName(""));
}


