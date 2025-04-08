#include "ARocket.h"

AARocket::AARocket()
{
 	
	PrimaryActorTick.bCanEverTick = false; // if ARocket should tick every frame

}

void AARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

