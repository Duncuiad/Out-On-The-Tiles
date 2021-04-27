// Copyright Epic Games, Inc. All Rights Reserved.

#include "OutOnTheTilesGameMode.h"
#include "OutOnTheTilesPlayerController.h"
#include "OutOnTheTilesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOutOnTheTilesGameMode::AOutOnTheTilesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AOutOnTheTilesPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void AOutOnTheTilesGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("HI, IT'S ME!"));
}