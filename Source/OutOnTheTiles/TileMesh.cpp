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

// Selects which subdivision to apply to face. If face is actually a tile, it doesn't do anything
// Subdivide is a member of TileMesh and not of Face because it appends the new mesh elements to the TileMesh instance
// Making it a member of Face would require to expose the TileMesh to the Face class, even though each face
// should only be aware of itself and its components (as well as its hierarchy)
void UTileMesh::Subdivide(UFace* face)
{
	if (face->IsSubdivided()) { return; }

	unsigned int depth = face->Depth();

	// If needed, subdivide the adjacent faces first
	this->CondSubdAdjacentOf(face);

	// Decide which subdivision to apply to the face, but only if it can be further subdivided
	if (depth < MAX_SUBDIVISION_DEPTH)
	{
		int type = face->FaceType();

		switch (type)
		{
		// Triangle
		case -1:
			this->SubdivideTriangle(face);
		// Red Square
		case 0:
			this->SubdivideRedSquare(face);
		// Red-Blue Square
		case 1:
			this->SubdivideRedBlueSquare(face);
		}
	}
}

void UTileMesh::CondSubdAdjacentOf(UFace* face)
{

	UHalfEdge* currentHE = face->getRepresentative();

	// iterate on the adjacent faces
	do
	{
		//if the half-edge has an opposite, the subdivision doesn't need to propagate in that direction
		if (!currentHE->IsInternal())
		{
			// if the half-edge doesnt't have a father (e. g. the face it lies on has no father face), it returns nullptr
			UHalfEdge* fatherHEdge = currentHE->FindFatherHEdge();

			if (fatherHEdge)
			{
				// propagate the subdivision to the adjacent face
				this->Subdivide(fatherHEdge->getFace());
			}
		}

		// go on to the next half-edge
		currentHE = currentHE->getNext();
	} while (currentHE != face->getRepresentative());
}

void UTileMesh::SubdivideTriangle(UFace* triangle)
{
}

void UTileMesh::SubdivideRedSquare(UFace* redSquare)
{
}

void UTileMesh::SubdivideRedBlueSquare(UFace* rbSquare)
{
}
