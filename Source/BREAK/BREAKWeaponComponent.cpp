// Copyright Epic Games, Inc. All Rights Reserved.


#include "BREAKWeaponComponent.h"
#include "BREAKCharacter.h"
#include "BREAKProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UBREAKWeaponComponent::UBREAKWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UBREAKWeaponComponent::Fire()
{
	if (!bCanFire || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is valid and we are about to spawn a projectile!"));

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	
			if (DoesSocketExist(FName("Muzzle")))
			{
				const FTransform MuzzleTransform = GetSocketTransform(FName("Muzzle"), RTS_World);
				const FVector MuzzleLocation = MuzzleTransform.GetLocation();

				
				FVector WorldLocation;
				FVector WorldDirection;
				// Get center of screen
				int32 ViewportSizeX, ViewportSizeY;
				PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
				FVector2D ScreenCenter(ViewportSizeX / 2.f, ViewportSizeY / 2.f);

				// Deproject screen center to world
				if (PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
				{
					// Line trace to get point of impact
					FVector EndLocation = WorldLocation + (WorldDirection * 10000.f); // Long ray

					// Compute direction to fire
					FVector FireDirection = (EndLocation - MuzzleLocation).GetSafeNormal();
					FRotator FireRotation = FireDirection.Rotation();

					// Set spawn params
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					ActorSpawnParams.Owner = GetOwner();

					// Spawn the projectile aimed toward crosshair
					World->SpawnActor<AARocket>(ProjectileClass, MuzzleLocation, FireRotation, ActorSpawnParams);

					bCanFire = false;
					World->GetTimerManager().SetTimer(FireCooldownTimer, this, &UBREAKWeaponComponent::CanFire, FIRE_COOLDOWN, false);
				}
			}
			
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool UBREAKWeaponComponent::AttachWeapon()
{
	
	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UBREAKWeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UBREAKWeaponComponent::Fire);
		}
	}

	return true;
}

void UBREAKWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABREAKCharacter>(GetOwner());
}

void UBREAKWeaponComponent::CanFire()
{
	bCanFire = true;
}

void UBREAKWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}