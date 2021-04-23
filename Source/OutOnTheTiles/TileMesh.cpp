// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMesh.h"

// Initialize the pointer to the unique instance of this class to be null (Singleton pattern)
UTileMesh* UTileMesh::instance = nullptr;

// Initialization of the inflation constant's value
const float UTileMesh::INFLATION_CONSTANT = sqrt(2.f + sqrt(3.f));

UTileMesh* UTileMesh::Instance()
{
	if (instance == nullptr)
	{
		UPROPERTY()
		TSubclassOf<UTileMesh> TileMesh;

		instance = NewObject<UTileMesh>(nullptr, TileMesh->GetFName(), RF_NoFlags, TileMesh.GetDefaultObject());
	}

	return instance;
}
