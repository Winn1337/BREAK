// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grappel.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UCableComponent;
class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class GRAPPLINGHOOK_API AGrappel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	UCableComponent* Cable;

	UPROPERTY()
	ACharacter* PlayerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Grapple")
	float PullStrength = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Grapple")
	float MinDistanceToPull = 50.f;

	UPROPERTY(EditAnywhere, Category = "Grapple")
	float MaxRopeLength = 1500.f;

	bool bHasHit;

	void HandlePlayerMovement(float DeltaTime);
};
