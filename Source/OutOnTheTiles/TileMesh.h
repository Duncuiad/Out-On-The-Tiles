// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Tile.h"

#include "MEList.h"

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
	static UTileMesh* instance;

	// The list of vertices in the mesh
	MEList<UVertex> vertices;

	// The list of half-edges in the mesh
	MEList<UHalfEdge> halfEdges;

	// The list of faces in the mesh, except for tiles (see below)
	MEList<UFace> faces;

	// The list of tiles in the mesh, i. e. the maximally subdivided faces.
	MEList<UTile> tiles;

public:

	// This is the value that lengths contract by at each subsequent subdivision
	const static float INFLATION_CONSTANT; // sqrt(2.f + sqrt(3.f))

	/** 
	* Returns a pointer to the unique instance of this class if present. In case it isn't, it first creates it (lazy initialization)
	* Note: this is kind of futile, since I can't make the default constructor protected (it has to be accessed by the factory method NewObject)
	* However the best practice for this class will be to NEVER call NewObject on the class outside of its definition
	*/
	static UTileMesh* Instance();

	// Selects which subdivision to apply to face. If face is actually a tile, it doesn't do anything
	void Subdivide(UFace* face);

	void SubdivideTriangle(UFace* triangle);

	void SubdivideRedSquare(UFace* redSquare);

	void SubdivideRedBlueSquare(UFace* rbSquare);
	
};
