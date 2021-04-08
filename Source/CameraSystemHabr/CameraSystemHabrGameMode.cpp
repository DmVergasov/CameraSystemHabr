// Copyright Epic Games, Inc. All Rights Reserved.

#include "CameraSystemHabrGameMode.h"
#include "CameraSystemHabrCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACameraSystemHabrGameMode::ACameraSystemHabrGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
