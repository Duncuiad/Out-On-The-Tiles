// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMesh.h"
#include <sstream>
#include <string>

// Initialize the pointer to the unique instance of this class to be null (Singleton pattern)
/* BROKEN: Unreal doesn't support static UObject members */
//UTileMesh* UTileMesh::instance = nullptr;

// Initialization of the inflation constant's value and other constants
const float UTileMesh::INFLATION_CONSTANT = sqrt(2.f + sqrt(3.f));
const float UTileMesh::COS_15_DEG = sqrt(2.f + sqrt(3.f)) / 2.f;
const float UTileMesh::SIN_15_DEG = sqrt(2.f - sqrt(3.f)) / 2.f;
const FMatrix2x2 UTileMesh::ELEM_ROT_MAT(COS_15_DEG, -SIN_15_DEG, COS_15_DEG, SIN_15_DEG);
const FMatrix2x2 UTileMesh::TR_ELEM_ROT_MAT(COS_15_DEG, SIN_15_DEG, COS_15_DEG, -SIN_15_DEG);

/* BROKEN: Unreal doesn't support static UObject members */
/*
UTileMesh* UTileMesh::Instance()
{
	if (UTileMesh::instance == nullptr)
	{
		//UPROPERTY()
		//TSubclassOf<UTileMesh> TileMesh;
		UE_LOG(LogTemp, Warning, TEXT("BUILDING TILEMESH FOR THE FIRST TIME"));
		//UTileMesh::instance = NewObject<UTileMesh>(world, TileMesh->GetFName(), RF_NoFlags, TileMesh.GetDefaultObject());
		UTileMesh::instance = NewObject<UTileMesh>();
	}
	return UTileMesh::instance;
}
*/

void UTileMesh::BuildFirstFaceTriangle(float sideLength)
{
	UPROPERTY() UVertex* vert[3];
	UPROPERTY() UHalfEdge* hedges[3];
	UPROPERTY() UFace* face;

	vert[0] = this->NewVertexAt(FVector2D(0.f, 0.f));
	vert[1] = this->NewVertexAt(FVector2D(sideLength, 0.f));
	vert[2] = this->NewVertexAt(FVector2D(sideLength / 2.f, sideLength / 2.f * sqrt(3.f)));

	hedges[0] = this->NewHalfEdgeAt(vert[0], false);
	hedges[1] = this->NewHalfEdgeAt(vert[1], true);
	hedges[2] = this->NewHalfEdgeAt(vert[2], true);

	UHalfEdge::MakeCycle(hedges[0], hedges[1], hedges[2]);

	face = this->NewFace(nullptr, hedges[0]);

	for (int i = 0; i < 3; i++)
	{
		hedges[i]->SetFace(face);
	}
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

void UTileMesh::GetInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Number of vertices: %d"), this->vertices.Size());
	UE_LOG(LogTemp, Warning, TEXT("Number of half-edges: %d"), this->halfEdges.Size());
	UE_LOG(LogTemp, Warning, TEXT("Number of faces: %d"), this->faces.Size());
	UE_LOG(LogTemp, Warning, TEXT("Number of tiles: %d"), this->tiles.Size());

	/*
	for (auto iterator = this->faces.begin(); iterator != this->faces.end(); ++iterator)
	{
		
		
		UE_LOG(LogTemp, Warning, TEXT("Un passo"));
		if (IsValid(*iterator))
		{
			UE_LOG(LogTemp, Warning, TEXT("Face depth: %d"), (*iterator)->Depth());
		}
		else
		{
			std::stringstream ss;
			ss << *iterator;
			std::string str = ss.str();
			UE_LOG(LogTemp, Warning, TEXT("\nIndirizzo non valido: %s\n"), str.c_str());
		}
		
	}
	*/
	
	for (UFace* face : this->faces)
	{
		UE_LOG(LogTemp, Warning, TEXT("Face depth: %d"), face->Depth());
	}
	
}

void UTileMesh::CondSubdAdjacentOf(UFace* face)
{

	UPROPERTY() UHalfEdge* currentHE = face->getRepresentative();

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

				// if it's not on the world border
				if (fatherHEdge->IsInternal())
				{
					// propagate the subdivision to the adjacent face
					this->Subdivide(fatherHEdge->getOpposite()->getFace());
				}
			}
		}

		// go on to the next half-edge
		currentHE = currentHE->getNext();
	} while (currentHE != face->getRepresentative());
}

UVertex* UTileMesh::NewVertexAt(const FVector2D& position)
{

	UPROPERTY() UVertex* newVertex;

	//UPROPERTY()
	//TSubclassOf<UVertex> Vertex;
	//newVertex = NewObject<UVertex>(this, Vertex->GetFName(), RF_NoFlags, Vertex.GetDefaultObject());
	newVertex = NewObject<UVertex>(this);

	// Initialize the vertex
	newVertex->setPosition(position);

	// Determine its marking
	newVertex->DetermineMarking();

	// Append to the TileMesh
	this->vertices.Append(newVertex);

	return newVertex;
}

UVertex* UTileMesh::GenerateSubMidpoint(UHalfEdge* halfEdge)
{
	UPROPERTY() UVertex* subMidpoint;

	// Get the vector corresponding to the halfEdge
	FVector2D HEDisplacement = halfEdge->getNext()->getBase()->getPosition() - halfEdge->getBase()->getPosition();

	// Rotate based on half-edge color
	FVector2D position = halfEdge->isBlue() ? UTileMesh::ELEM_ROT_MAT.TransformVector(HEDisplacement) : UTileMesh::TR_ELEM_ROT_MAT.TransformVector(HEDisplacement);

	// Scale and translate to get position of subMidpoint
	position /= UTileMesh::INFLATION_CONSTANT;
	position += halfEdge->getBase()->getPosition();

	// Generate the vertex and append it to the TileMesh
	subMidpoint = this->NewVertexAt(position);

	halfEdge->SetSubMidpoint(subMidpoint);

	return subMidpoint;
}

UHalfEdge* UTileMesh::NewHalfEdgeAt(UVertex* baseVertex, bool shouldBeBlue)
{
	UPROPERTY() UHalfEdge* newHalfEdge;

	//UPROPERTY()
	//TSubclassOf<UHalfEdge> HalfEdge;
	//newHalfEdge = NewObject<UHalfEdge>(this, HalfEdge->GetFName(), RF_NoFlags, HalfEdge.GetDefaultObject());
	newHalfEdge = NewObject<UHalfEdge>(this);

	newHalfEdge->Init(baseVertex, shouldBeBlue);

	this->halfEdges.Append(newHalfEdge);

	return newHalfEdge;
}

UFace* UTileMesh::NewFace(UFace* father, UHalfEdge* candidateRepresentative)
{
	UPROPERTY() UFace* newFace;

	//UPROPERTY()
	//TSubclassOf<UFace> Face;
	//newFace = NewObject<UFace>(this, Face->GetFName(), RF_NoFlags, Face.GetDefaultObject());
	newFace = NewObject<UFace>(this);

	// candidateRepresentative could be any of the face's half-edges
	// This part makes sure that the correct representative is picked
	UHalfEdge* representative = candidateRepresentative->FindFirstRedFromMe();

	newFace->Init(father, representative);

	this->faces.Append(newFace);

	return newFace;
}

void UTileMesh::SubdivideTriangle(UFace* triangle)
{
	UPROPERTY() UVertex* originalVertices[3];
	UPROPERTY() UVertex* subMidpoints[3];
	UPROPERTY() UHalfEdge* inner[4];
	UPROPERTY() UHalfEdge* border[6];
	UPROPERTY() UFace* newFaces[3];
	UPROPERTY() UHalfEdge* currentHE = triangle->getRepresentative();

	// collect original vertices and generate new ones where needed
	// then generate the border half-edges, permorming conditional pairing with opposites (see documentation [when available])
	for (int i = 0; i < 3; i++)
	{
		// if the adjacent face exists (this half-edge is not on the world border) and has already been subdivided, it generated the subMidpoint for this half-edge in the process
		bool adjacentFaceAlreadySubdivided = (bool) currentHE->GetSubMidpoint();

		originalVertices[i] = currentHE->getBase();

		// If the subMidpoint was already generated by the adjacent face's subdivision, get its reference
		// Otherwise first generate it and append its reference to the TileMesh
		// note that subMidpoint marking is done at the moment of generation
		subMidpoints[i] = adjacentFaceAlreadySubdivided ? currentHE->GetSubMidpoint() : this->GenerateSubMidpoint(currentHE);

		// generate the border half-edges
		border[2 * i] = this->NewHalfEdgeAt(originalVertices[i], !currentHE->isBlue());
		border[2 * i + 1] = this->NewHalfEdgeAt(subMidpoints[i], !currentHE->isBlue());

		// pair (border-)half-edges' opposites where possible and notify father's opposites when not

		if (adjacentFaceAlreadySubdivided)
		{ // I retrieve the information concerning currentHE's opposites and do the pairing

			border[2 * i]->PairOpposites(currentHE->GetCachedChild());
			border[2 * i + 1]->PairOpposites(currentHE->getOpposite()->GetCachedChild());

		}
		else
		{ // I need to store currentHE's children so that its opposite will be able to determine its children opposites

			// If the current half-edge is not on the world border
			if (currentHE->getOpposite())
			{

				// Store the needed information
				currentHE->CacheSubdivisionInformation(border[2 * i], border[2 * i + 1]);

			}
			
		}

		currentHE = currentHE->getNext();
	}

	// generate the inner half-edges and pair them
	inner[0] = this->NewHalfEdgeAt(subMidpoints[0], true);
	inner[1] = this->NewHalfEdgeAt(subMidpoints[2], false);
	inner[0]->PairOpposites(inner[1]);

	inner[2] = this->NewHalfEdgeAt(subMidpoints[0], false);
	inner[3] = this->NewHalfEdgeAt(subMidpoints[1], true);
	inner[2]->PairOpposites(inner[3]);

	// set each half-edge's next

	// - first triangle
	UHalfEdge::MakeCycle(border[5], border[0], inner[0]);

	// - second triangle
	UHalfEdge::MakeCycle(border[2], inner[3], border[1]);

	// - square
	UHalfEdge::MakeCycle(inner[1], inner[2], border[3], border[4]);

	// pair (border) half-edges' opposites where possible and notify father's opposites when not
	// TODO, probably in the generation cycle above

	// apply half-edge color changing based on original vertices markings
	// this must be done AFTER the half-edge linking has happened
	for (int i = 0; i < 3; i++)
	{
		border[2 * i + 1]->ApplyEndpointMarking();
	}

	// determine new faces together with their representative half-edges and set them as children of triangle
	newFaces[0] = this->NewFace(triangle, border[5]); // first triangle
	newFaces[1] = this->NewFace(triangle, border[2]); // second triangle
	newFaces[2] = this->NewFace(triangle, inner[1]); // square

	// set the face field of each half-edge
	for (int i = 0; i < 3; i++)
	{
		UPROPERTY() UHalfEdge* current = newFaces[i]->getRepresentative();
		do
		{
			current->SetFace(newFaces[i]);

			current = current->getNext();
		} while (current != newFaces[i]->getRepresentative());
	}
}

void UTileMesh::SubdivideRedSquare(UFace* redSquare)
{
}

void UTileMesh::SubdivideRedBlueSquare(UFace* rbSquare)
{
}
