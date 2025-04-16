// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grappel.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UCableComponent;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Check if hook has attached to a surface
	bool IsHooked() const { return bHasHit; }

	// Get the location of the hook (for player to pull towards)
	FVector GetHookLocation() const { return GetActorLocation(); }

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	UCableComponent* Cable;

	bool bHasHit;
};
