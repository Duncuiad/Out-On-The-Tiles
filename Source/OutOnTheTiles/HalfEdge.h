// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vertex.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HalfEdge.generated.h"

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

	// HE Color information to determine how this half-edge should be subdivided
	bool blue;

public:

	inline UVertex* getBase() const { return this->base; }
	inline UHalfEdge* getNext() const { return this->next; }
	inline const bool isBlue() { return blue; };

};
