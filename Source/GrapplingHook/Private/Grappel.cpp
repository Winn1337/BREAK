// Fill out your copyright notice in the Description page of Project Settings.


#include "Grappel.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	ProjectileMovement->InitialSpeed = 12000.f;
	ProjectileMovement->MaxSpeed = 12000.f;
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

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		Cable->AttachToComponent(PlayerCharacter->GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
		MovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

// Called every frame
void AGrappel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasHit)
	{
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 100.f;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			if (Hit.GetActor() && Hit.GetActor()->ActorHasTag("GrappleSurface"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Grapple attached to surface at %s"), *Hit.ImpactPoint.ToString());

				ProjectileMovement->StopMovementImmediately();
				SetActorLocation(Hit.ImpactPoint);

				bHasHit = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Grapple hit non-grapple surface. Destroying."));
				Destroy();
			}
		}
	}
	else
	{
		HandlePlayerMovement(DeltaTime);
	}
}

void AGrappel::HandlePlayerMovement(float DeltaTime)
{
	if (!PlayerCharacter || !MovementComponent) return;

	FVector HookLocation = GetActorLocation();
	FVector ToHook = HookLocation - PlayerCharacter->GetActorLocation();
	float Distance = ToHook.Size();

	if (Distance > MinDistanceToPull)
	{
		if (MovementComponent->MovementMode != MOVE_Falling)
		{
			MovementComponent->SetMovementMode(MOVE_Falling);
		}

		ToHook.Normalize();
		PlayerCharacter->LaunchCharacter(ToHook * PullStrength * DeltaTime, true, true);
	}
}

