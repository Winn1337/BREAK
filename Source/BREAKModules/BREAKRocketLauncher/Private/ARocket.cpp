// Fill out your copyright notice in the Description page of Project Settings.


#include "ARocket.h"
#include "Components/StaticMeshComponent.h"

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
	RocketProjectileMovement->bRotationFollowsVelocity = true;
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
	if (OtherActor && OtherActor != this)
	{
		// Spawn the explosion
		if (RocketExplosion)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			/////// FIX THIS, THIS CRASHES THE UNREAL EDITOR
			AARocketExplosion* Explosion = GetWorld()->SpawnActor<AARocketExplosion>(
				RocketExplosion,
				GetActorLocation(),         // explosion position
				FRotator::ZeroRotator,      // no rotation needed
				spawnParams
			);

			if (Explosion)
			{
				Explosion->TriggerExplosion();
			}
		}

		Destroy();
	}
}

#pragma endregion


