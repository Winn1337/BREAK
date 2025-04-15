// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocket.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ARocketExplosion.h"

// Sets default values
AARocket::AARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// Components instantiations

	RocketSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RocketSceneComponent"));
	RootComponent = RocketSceneComponent;

	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);

	RocketCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RocketCollision"));
	RocketCollision->SetupAttachment(RocketMesh);
	RocketCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	RocketCollision->SetGenerateOverlapEvents(true);

	RocketProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("RocketProjectileMovement"));
	RocketProjectileMovement->bAutoActivate = true;
	RocketProjectileMovement->InitialSpeed = 50;
	RocketProjectileMovement->MaxSpeed = 100;
	RocketProjectileMovement->bRotationFollowsVelocity = false;
	RocketProjectileMovement->bShouldBounce = false;
	
	// Member variable instantiations

	// Register events
	RocketCollision->OnComponentBeginOverlap.AddDynamic(this, &AARocket::OnCollisionOverlap);
}

// Called when the game starts or when spawned
void AARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region EVENTS

void AARocket::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		
		if (OtherComp->GetCollisionObjectType() == ECC_Pawn)
		{
			return;
		}

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FVector SpawnLocation = GetActorLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FRotator spawnRotation = FRotator::ZeroRotator;
			// Spawn the projectile at the muzzle
			AARocketExplosion* Explosion = World->SpawnActor<AARocketExplosion>(RocketExplosion, SpawnLocation, spawnRotation, ActorSpawnParams);

			if (Explosion != nullptr)
			{
				Explosion->TriggerExplosion();
			}
		}

		Destroy();
	}
}

#pragma endregion


