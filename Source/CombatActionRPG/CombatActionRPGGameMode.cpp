// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatActionRPGGameMode.h"
#include "CombatActionRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACombatActionRPGGameMode::ACombatActionRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
