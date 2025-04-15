// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "BREAKModules/BREAKRocketLauncher/Private/ARocketExplosion.h"
#include "ARocket.generated.h"

UCLASS()
class BREAKROCKETLAUNCHER_API AARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Functions

	// event function for overlap collisions
	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// Classes

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TSubclassOf<AARocketExplosion> RocketExplosion;

	// Components

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RocketSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Rocket Mesh")
	UStaticMeshComponent* RocketMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Rocket Movmement")
	UProjectileMovementComponent* RocketProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	UCapsuleComponent* RocketCollision;

	// Member variables

	// Const 

	static constexpr float ROCKET_TIME_TO_LIVE = 3;
};
