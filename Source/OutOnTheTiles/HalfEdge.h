// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HalfEdge.generated.h"

// - Forward declarations
class UVertex; //#include "Vertex.h"
class UFace; //#include "Face.h"

/**
 * A class for the half-edges (HE) in the tiling (stored as a half-edge mesh)
 * It stores topological information as the base vertex, the next half-edge on the same face and the opposite half-edge (same edge, other face) if present
 * It also stores the color of the half edge (red/blue), which is needed to determine how to subdivide the tiling
 */
UCLASS()
class OUTONTHETILES_API UHalfEdge : public UObject
{
	GENERATED_BODY()

	// The Vertex this Half-Edge comes out of
	UVertex* base;

	// The next Half-Edge on this HE's Face. The orientation should be positive (i. e. a full HE cycle should be oriented counter-clockwise)
	UHalfEdge* next;

	// The corresponding Half-Edge on the same edge of the adjacent face, if present
	UHalfEdge* opposite = nullptr;

	// This half-edge's face
	UFace* face;

	// HE Color information to determine how this half-edge should be subdivided
	bool blue;

	// Each subdivision step subdivides each half-edge into to smaller half-edges, introducing a displaced midpoint vertex
	UVertex* subMidpoint = nullptr;

public:

	// - get functions
	inline UVertex* getBase() const { return this->base; }
	inline UHalfEdge* getNext() const { return this->next; }
	inline UVertex* getEnd() const { return this->getNext()->getBase(); }
	inline UHalfEdge* getOpposite() const { return this->opposite; }
	inline UFace* getFace() const { return this->face; }
	inline bool isBlue() const { return this->blue; }
	inline UVertex* GetSubMidpoint() const { return this->subMidpoint; }

	// True if this half-edge has an opposite, false if it is instead on the border of its mesh (the submesh corresponding to the depth of the face it belongs to)
	inline bool IsInternal() const { return (bool) this->opposite; }

	inline void ChangeColor() { this->blue = !this->blue; }

	// EDIT COMMENT
	UHalfEdge* FindFatherHEdge();

	// If a vertex is marked, all of its incident half-edges should have opposite color to their default state
	// This function is called by any half-edge ending in the vertex to be checked, and if such vertex is marked it changes color to itself and its next half-edge
	// (which is always a half-edge starting from the checked vertex)
	void ApplyEndpointMarking();

	// Sets the subdivision midpoint of the half-edge and of its opposite at the same time
	inline void SetSubMidpoint(UVertex* calculatedSubMidpoint)
	{
		this->subMidpoint = calculatedSubMidpoint;
		if (this->opposite)
		{
			this->opposite->subMidpoint = calculatedSubMidpoint;
		}
	}

};
