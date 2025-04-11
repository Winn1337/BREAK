// Fill out your copyright notice in the Description page of Project Settings.


#include "Grappel.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGrappel::AGrappel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Hook mesh
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(Root);

	// Optional: Set a default static mesh if you want in code
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> HookVisualAsset(TEXT("StaticMesh'/Game/Path/To/Your/Hook.Hook'"));
	// if (HookVisualAsset.Succeeded()) { HookMesh->SetStaticMesh(HookVisualAsset.Object); }

	// Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AGrappel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrappel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

