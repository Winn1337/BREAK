// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocketExplosion.h"

// Sets default values
AARocketExplosion::AARocketExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component initialization

	ExplosionSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionSceneComponent"));
	RootComponent = ExplosionSceneComponent;

	ExplosionRadiusCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRadiusCollider"));
	ExplosionRadiusCollider->SetupAttachment(RootComponent);
	ExplosionRadiusCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	ExplosionRadiusCollider->SetGenerateOverlapEvents(true);

	// Member variable initialization
	CenterOfExplosion = GetActorLocation();
	ExplosionForce = 5000;
}

// Called when the game starts or when spawned
void AARocketExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARocketExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARocketExplosion::TriggerExplosion()
{
	TArray<AActor*> overlappingActors;
	ExplosionRadiusCollider->GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		if (actor && actor != this)
		{
			FVector actorLocation = actor->GetActorLocation();
			FVector direction = actorLocation - GetActorLocation();
			float distance = direction.Size();

			if (distance == 0) continue; // prevent divide-by-zero

			direction.Normalize();

			float forceMagnitude = FMath::Clamp(ExplosionForce / distance, 0.0f, ExplosionForce);

			UPrimitiveComponent* primitiveComp = Cast<UPrimitiveComponent>(actor->GetRootComponent());
			if (primitiveComp && primitiveComp->IsSimulatingPhysics())
			{
				primitiveComp->AddImpulse(direction * forceMagnitude, NAME_None, true);
			}
		}
	}
}

#pragma region Events

void AARocketExplosion::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		FVector otherActorLocation = OtherActor->GetActorLocation();

		FVector directionToExplosionCenter = otherActorLocation - CenterOfExplosion;
		
		float distanceFromExplosionCenter = directionToExplosionCenter.Size();

		directionToExplosionCenter.Normalize();

		float forceMagnitude = FMath::Clamp(ExplosionForce / distanceFromExplosionCenter, 0.0, ExplosionForce); // Force magnitude based on distance from explosion center

		UPrimitiveComponent* otherActorPrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (otherActorPrimitiveComponent)
		{
			otherActorPrimitiveComponent->AddImpulse(-directionToExplosionCenter * forceMagnitude, NAME_None, true);
		}
	}
}

#pragma endregion



