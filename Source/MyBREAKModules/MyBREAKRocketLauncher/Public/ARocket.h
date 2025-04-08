// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARocket.generated.h"

UCLASS()
class MYBREAKROCKETLAUNCHER_API AARocket : public AActor
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

};
