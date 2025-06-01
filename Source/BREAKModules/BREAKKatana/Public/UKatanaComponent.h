// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "UKatanaComponent.generated.h"

class ACharacter;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BREAKKATANA_API UUKatanaComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* KatanaMappingContext;

	/** Swing Katana Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingKatanaAction;

	/** Sound to play each time we swing the katana */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* SwingKatanaSound;

	/** AnimMontage to play each time we swing the katana */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* SwingKatanaAnimation;

	/** Sets default values for this component's properties */
	UUKatanaComponent();


protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Set the character and mesh in character class
	/// </summary>
	/// <param name="character"></param>
	/// <param name="mesh1P"></param>
	/// <returns></returns>

	UFUNCTION(BlueprintCallable, Category = "Katana")
	bool SetCharacterAndSkeletalMesh(ACharacter* character, USkeletalMeshComponent* mesh1P);

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Katana")
	bool AttachKatana(ACharacter* character, USkeletalMeshComponent* mesh1P);

private:

	// Functions

	/** Swing the katana */
	UFUNCTION(BlueprintCallable, Category = "Katana")
	void SwingKatana();

	void CanSwingKatana();

	void OnKatanaSwingMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	// Components

	UPROPERTY(VisibleAnywhere)
	USceneComponent* KatanaSceneComponent;

	// Member variables


	/// <summary>
	/// Set these from character class when you attach it with SetCharacterAndSkeletalMesh
	/// </summary>
	ACharacter* Character;
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly, Category = "Katana properties")
	float KatanaDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Katana properties")
	float KatanaSlashRange;

	bool BCanSwingKatana = true;
	FTimerHandle SwingCooldownTimer;
	static constexpr float SWING_COOLDOWN = 0.4;
	
};
