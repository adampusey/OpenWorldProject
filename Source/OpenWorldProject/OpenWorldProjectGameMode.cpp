// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldProjectGameMode.h"
#include "OpenWorldProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOpenWorldProjectGameMode::AOpenWorldProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
