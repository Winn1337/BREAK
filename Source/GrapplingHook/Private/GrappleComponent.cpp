// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGrappleComponent::UGrappleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrappleComponent::FireGrapple()
{
	bIsGrappleHooked = false;
	
	if (GrappleClass && !ActiveGrapple)
	{
		AActor* Owner = GetOwner();
		if (!Owner) return;

		// Get player controller & camera location/direction
		APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
		if (!PC) return;

		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.f);

		// Raycast to find hit point
		FHitResult Hit;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(Owner);

		FVector TargetLocation = TraceEnd;

		// Raycast to find Grappleable surface
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			//TargetLocation = Hit.ImpactPoint;

			if (Hit.GetActor() && Hit.GetActor()->ActorHasTag("GrappleSurface"))
			{
				TargetLocation = Hit.ImpactPoint;
				bIsGrappleHooked = true;
				UE_LOG(LogTemp, Warning, TEXT("Hit GrappleSurface at %s"), *TargetLocation.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit non-grapple surface: %s"), *Hit.GetActor()->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No hit — firing grapple straight ahead"));
		}

		// Direction to the target point
		FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;
		FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;

		ActiveGrapple = GetWorld()->SpawnActor<AActor>(GrappleClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void UGrappleComponent::ReleaseGrapple()
{
	if (ActiveGrapple)
	{
		ActiveGrapple->Destroy();
		ActiveGrapple = nullptr;
	}

	bIsGrappleHooked = false;
}

void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ActiveGrapple || !bIsGrappleHooked) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	FVector HookLocation = ActiveGrapple->GetActorLocation();
	FVector PlayerLocation = OwnerCharacter->GetActorLocation();
	FVector ToHook = HookLocation - PlayerLocation;
	float Distance = ToHook.Size();

	if (Distance < 10.f) return;

	ToHook.Normalize();

	// Set movement mode if needed
	if (OwnerCharacter->GetCharacterMovement()->MovementMode != MOVE_Falling)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}

	// Reel player in
	float PullSpeed = 3000.f;
	OwnerCharacter->LaunchCharacter(ToHook * PullSpeed * DeltaTime, false, false);
}

