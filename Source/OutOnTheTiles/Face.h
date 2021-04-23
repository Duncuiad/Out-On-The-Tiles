// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vertex.h"
#include "HalfEdge.h"
#include "Containers/List.h"

#include "Math/Vector2D.h"
#include "Math/TransformCalculus2D.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Face.generated.h"

/**
 * 
 */
UCLASS()
class OUTONTHETILES_API UFace : public UObject
{
	GENERATED_BODY()

	// The Face keeps a reference to one of its half-edges. More information (like its vertices or the other HEs) can be accessed via rep's interface
	UHalfEdge* rep;

	// Points to the face's father in the subdivision tree
	UFace* father;

	// Pointers to the face's children in the subdivision tree
	TDoubleLinkedList<UFace*> children;

public:

	// Class initializer (use after a call of NewObject<Uface>)
	inline void InitFace(UHalfEdge& representative, UFace& fatherFace) { this->rep = &representative; this->father = &fatherFace; }

	// Returns the depth of this face in the subdivision tree. Root has depth one (for calculation purposes). Children have their father's depth +1
	unsigned int Depth() const;

	// Returns -1 if the face is a triangle, 0 if it is a red square and 1 if it is a red-blue square
	int FaceType() const;

	// Returns the position of the face's first vertex (i. e. the base vertex of the representative half-edge)
	inline FVector2D getPosition() const { return this->rep->getBase()->getPosition(); }

	// Returns the 2D transform (rotation and scale only) of this face with respect to its "canonical form", where the representative half-edge corresponds to the vector (1,0)
	FMatrix2x2 getTransform() const;
};