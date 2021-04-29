// Copyright Epic Games, Inc. All Rights Reserved.

#include "TileMesh.h"

#include <sstream>
#include <string>

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

	UPROPERTY() UTileMesh* mesh = NewObject<UTileMesh>(this);

	UE_LOG(LogTemp, Warning, TEXT("TILEMESH CREATED"));

	mesh->GetInfo();

	mesh->BuildFirstFaceTriangle(1000.f);

	UE_LOG(LogTemp, Warning, TEXT("FIRST TRIANGLE CREATED"));

	mesh->GetInfo();

	UFace* firstFace = mesh->GetRootFace();

	if (firstFace == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FACE IS NULL"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I HAVE THE FIRST FACE"));
		
		std::stringstream ss;
		ss << firstFace;
		std::string str = ss.str();
		UE_LOG(LogTemp, Warning, TEXT("\n%s\n"), str.c_str());
		
	}

	mesh->Subdivide(firstFace);

	UE_LOG(LogTemp, Warning, TEXT("SUCCESFULLY SUBDIVIDED"));

	mesh->GetInfo();
}