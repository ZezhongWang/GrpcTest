// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrpcTestGameMode.h"
#include "GrpcTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrpcTestGameMode::AGrpcTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
