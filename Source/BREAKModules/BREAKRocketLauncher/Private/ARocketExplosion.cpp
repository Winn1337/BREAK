// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocketExplosion.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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
	TArray<UPrimitiveComponent*> overlappingComponents;
	ExplosionRadiusCollider->GetOverlappingComponents(overlappingComponents);

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadiusCollider->GetScaledSphereRadius(), 32, FColor::Red, false, 2.0f);

	for (UPrimitiveComponent* comp : overlappingComponents)
	{
		if (!comp || comp == this->GetRootComponent()) continue;

		AActor* actor = comp->GetOwner();
		if (!actor || actor == this) continue;

		if (ACharacter* character = Cast<ACharacter>(actor))
		{
			FVector direction = character->GetActorLocation() - GetActorLocation();
			float distance = direction.Size();
			if (distance == 0.0f) continue;

			direction.Normalize();

			float forceMagnitude = FMath::Clamp(ExplosionForce / distance, 0.0f, ExplosionForce);

			// This is the real magic
			FVector impulse = direction * forceMagnitude;

			// Combine current velocity for momentum stacking
			FVector finalLaunchVelocity = character->GetVelocity() + impulse;

			UE_LOG(LogTemp, Warning, TEXT("Rocket-jumping character %s with velocity %s"), *character->GetName(), *finalLaunchVelocity.ToString());

			// This launches the character while preserving XY/Z control
			character->LaunchCharacter(finalLaunchVelocity, true, true);

			continue;
		}

		// For physics-simulated objects
		if (comp->IsSimulatingPhysics())
		{
			FVector componentLocation = comp->GetComponentLocation();
			FVector direction = componentLocation - GetActorLocation();
			float distance = direction.Size();
			if (distance == 0.0f) continue;

			direction.Normalize();
			float forceMagnitude = FMath::Clamp(ExplosionForce / distance, 0.0f, ExplosionForce);

			comp->AddImpulse(direction * forceMagnitude, NAME_None, true);
		}
	}
}

#pragma region Events

#pragma endregion



