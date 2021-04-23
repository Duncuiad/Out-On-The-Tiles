// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"

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

	// The list of vertices in the mesh

	// The list of half-edges in the mesh

	// The list of faces in the mesh, except for tiles (see below)

	// The list of tiles in the mesh, i. e. the maximally subdivided faces.

public:

	// This is the value that lengths contract by at each subsequent subdivision
	const static float INFLATION_CONSTANT; // sqrt(2 + sqrt(3))
	
};
