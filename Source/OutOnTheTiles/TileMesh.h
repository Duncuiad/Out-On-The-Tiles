// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Tile.h"

#include "MEList.h"

#include "Math/Vector2D.h"
#include "Math/TransformCalculus2D.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TileMesh.generated.h"

/**
 * 
 */
UCLASS()
class OUTONTHETILES_API UTileMesh : public UObject
{
	GENERATED_BODY()

	// Maximum number of subdivisions to allow (including the root)
	const static int MAX_SUBDIVISION_DEPTH = 8;

	// Pointer to the unique instance of this class (Singleton pattern)
	/* BROKEN: Unreal doesn't support static UObject members */
	//static UPROPERTY() UTileMesh* instance;

	// The list of vertices in the mesh
	MEList<UPROPERTY() UVertex*> vertices;

	// The list of half-edges in the mesh
	MEList<UPROPERTY() UHalfEdge*> halfEdges;

	// The list of faces in the mesh, except for tiles (see below)
	MEList<UPROPERTY() UFace*> faces;

	// The list of tiles in the mesh, i. e. the maximally subdivided faces.
	MEList<UPROPERTY() UTile*> tiles;

public:

	// This is the value that lengths contract by at each subsequent subdivision
	const static float INFLATION_CONSTANT; // sqrt(2.f + sqrt(3.f))
	
	// Variables for linear algebra on the mesh elements
	const static float COS_15_DEG; // sqrt(2.f + sqrt(3.f)) / 2.f
	const static float SIN_15_DEG; // sqrt(2.f - sqrt(3.f)) / 2.f
	// Rotation matrix used to determine subdivision mid-points of half-edges
	const static FMatrix2x2 ELEM_ROT_MAT; // (COS_15_DEG, -SIN_15_DEG; COS_15_DEG, SIN_15_DEG)
	const static FMatrix2x2 TR_ELEM_ROT_MAT; // (COS_15_DEG, SIN_15_DEG; COS_15_DEG, -SIN_15_DEG)

	/** 
	* Returns a pointer to the unique instance of this class if present. In case it isn't, it first creates it (lazy initialization)
	* Note: this is kind of futile, since I can't make the default constructor protected (it has to be accessed by the factory method NewObject)
	* However the best practice for this class will be to NEVER call NewObject on the class outside of its definition
	*/
	/* BROKEN: Unreal doesn't support static UObject members */
	//static UTileMesh* Instance();

	inline UFace* GetRootFace()
	{
		return *(this->faces.begin());
	}

	void BuildFirstFaceTriangle(float sideLength);

	// Selects which subdivision to apply to face. If face is actually a tile, it doesn't do anything
	void Subdivide(UFace* face);

	// Prints the number of elements in each of TileMesh's lists
	inline void GetInfo()
	{
		UE_LOG(LogTemp, Warning, TEXT("Number of vertices: %d"), this->vertices.Size());
		UE_LOG(LogTemp, Warning, TEXT("Number of half-edges: %d"), this->halfEdges.Size());
		UE_LOG(LogTemp, Warning, TEXT("Number of faces: %d"), this->faces.Size());
		UE_LOG(LogTemp, Warning, TEXT("Number of tiles: %d"), this->tiles.Size());
	}

private:

	// Check if the adjacent faces are sufficiently subdivided, otherwise subdivide them first
	// Stands for "conditionally subdivide (father-)adjacent faces of:"
	void CondSubdAdjacentOf(UFace* face);

	UVertex* NewVertexAt(const FVector2D& position);

	UVertex* GenerateSubMidpoint(UHalfEdge* halfEdge);

	UHalfEdge* NewHalfEdgeAt(UVertex* baseVertex, bool shouldBeBlue);

	UFace* NewFace(UFace* father, UHalfEdge* candidateRepresentative);

	void SubdivideTriangle(UFace* triangle);

	void SubdivideRedSquare(UFace* redSquare);

	void SubdivideRedBlueSquare(UFace* rbSquare);
	
};
