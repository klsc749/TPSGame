// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSGameGameModeBase.h"

#include "Player/BaseCharacter.h"
#include "Player/TPSPlayerController.h"


ATPSGameGameModeBase::ATPSGameGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
}
