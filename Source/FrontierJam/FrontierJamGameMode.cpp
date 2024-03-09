// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrontierJamGameMode.h"
#include "FrontierJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFrontierJamGameMode::AFrontierJamGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
