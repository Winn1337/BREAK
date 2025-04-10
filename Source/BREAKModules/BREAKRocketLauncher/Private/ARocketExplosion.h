// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "ARocketExplosion.generated.h"

UCLASS()
class AARocketExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARocketExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	// Functions

	// Handles the explosion logic
	UFUNCTION()
	void TriggerExplosion();

private:

	// Functions
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// Components

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ExplosionSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Explosion Radius")
	USphereComponent* ExplosionRadiusCollider;

	// Member variables

	FVector CenterOfExplosion;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionForce;

};
