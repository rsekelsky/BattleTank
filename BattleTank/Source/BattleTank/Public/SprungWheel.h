// Copyright Rourke Sekelsky.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

class USphereComponent;
class UPhysicsConstraintComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	ASprungWheel();

	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float ForceMagnitude);

protected:
	virtual void BeginPlay() override;

private:
	void ApplyForce();

	void SetupConstraints();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* SuspensionConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* AxleConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Axle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Wheel = nullptr;

	float TotalForceMagnitudeThisFrame = 0;
};
