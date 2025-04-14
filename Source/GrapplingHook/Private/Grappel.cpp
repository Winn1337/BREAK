// Fill out your copyright notice in the Description page of Project Settings.


#include "Grappel.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGrappel::AGrappel()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(Root);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(Root); // neutral attachment
	Cable->bAttachStart = true;
	Cable->bAttachEnd = true;

	bHasHit = false;
}

// Called when the game starts or when spawned
void AGrappel::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* MyPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyPlayerCharacter && Cable)
	{
		// Attach the cable component to the player’s capsule
		Cable->AttachToComponent(MyPlayerCharacter->GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);

		// Ensure it uses the correct ends
		Cable->bAttachStart = true;
		Cable->bAttachEnd = true;

		// Optional, just for safety
		Cable->EndLocation = FVector::ZeroVector;
	}
}

// Called every frame
void AGrappel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasHit) return;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (Hit.GetActor())
		{
			if (Hit.GetActor()->ActorHasTag("GrappleSurface"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Grapple attached to surface at %s"), *Hit.ImpactPoint.ToString());

				ProjectileMovement->StopMovementImmediately();
				SetActorLocation(Hit.ImpactPoint);

				bHasHit = true;
			}
			else
			{
				// Hit something that's not grappleable
				UE_LOG(LogTemp, Warning, TEXT("Grapple hit non-grapple surface. Destroying."));
				Destroy();
			}
		}
	}
}

