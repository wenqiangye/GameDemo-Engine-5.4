// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameDemoGameModeBase.h"
#include "Player/GameDemoCharacter.h"
#include "Player/GameDemoPlayerController.h"
#include "UI/GDGameHUD.h"

AGameDemoGameModeBase::AGameDemoGameModeBase()
{
	DefaultPawnClass = AGameDemoCharacter::StaticClass();
	PlayerControllerClass = AGameDemoPlayerController::StaticClass();
	HUDClass = AGDGameHUD::StaticClass();
}
