// Fill out your copyright notice in the Description page of Project Settings.
DEFINE_LOG_CATEGORY_STATIC(LogKatana, Log, All);


#include "UKatanaComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"


UUKatanaComponent::UUKatanaComponent()
{
}

void UUKatanaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(KatanaMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

void UUKatanaComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	
}

bool UUKatanaComponent::SetCharacterAndSkeletalMesh(ACharacter* character, USkeletalMeshComponent* mesh1P)
{
	if (character && mesh1P)
	{
		Character = character;
		Mesh1P = mesh1P;

		return true;
	}

	return false;
}

bool UUKatanaComponent::AttachKatana(ACharacter* character, USkeletalMeshComponent* mesh1P)
{
	// Check that the character is valid, and has no weapon component yet
	if (character == nullptr || character->GetInstanceComponents().FindItemByClass<UUKatanaComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(mesh1P, AttachmentRules, FName(TEXT("KatanaGripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Swing katana action when using touch input
			Subsystem->AddMappingContext(KatanaMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Swing Katana
			EnhancedInputComponent->BindAction(SwingKatanaAction, ETriggerEvent::Triggered, this, &UUKatanaComponent::SwingKatana);
		}
	}

	return true;
}

void UUKatanaComponent::SwingKatana()
{
	if (!BCanSwingKatana || !Character || !Mesh1P)
	{
		return;
	}
	SetVisibility(true, true);
	// Try and play the swing katana sound if specified
	if (SwingKatanaSound != nullptr)
	{

		UGameplayStatics::PlaySoundAtLocation(this, SwingKatanaSound, Character->GetActorLocation());
	}

	// Try and play a swing katana animation if specified
	if (SwingKatanaAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UUKatanaComponent::OnKatanaSwingMontageEnded);

			AnimInstance->Montage_Play(SwingKatanaAnimation, 1.f);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, SwingKatanaAnimation);
		}
	}
	UWorld* const World = GetWorld();
	BCanSwingKatana = false;
	World->GetTimerManager().SetTimer(SwingCooldownTimer, this, &UUKatanaComponent::CanSwingKatana, SWING_COOLDOWN, false);
}

void UUKatanaComponent::OnKatanaSwingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SwingKatanaAnimation)
	{
		UE_LOG(LogTemp, Log, TEXT("Katana swing animation finished. Interrupted: %s"), bInterrupted ? TEXT("Yes") : TEXT("No"));
	
	}
	BCanSwingKatana = true;
	SetVisibility(false, true);
}


void UUKatanaComponent::CanSwingKatana()
{
	//BCanSwingKatana = true;

}
