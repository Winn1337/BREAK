// Copyright Epic Games, Inc. All Rights Reserved.

#include "BREAKGameMode.h"
#include "BREAKCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABREAKGameMode::ABREAKGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
