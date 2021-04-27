// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfEdge.h"
#include "Vertex.h"
#include "Face.h"
#include "TileMesh.h"

UHalfEdge* UHalfEdge::FindFatherHEdge()
{
	UHalfEdge* fatherHEdge = nullptr;

	// check if its face has a father, otherwise the half-edge can't have a father half-edge
	if (this->getFace()->Depth() > 1)
	{
		UHalfEdge* candidate = this->getFace()->getFather()->getRepresentative();

		// iterate on the father face's half-edges
		// a father half-edges shares either the basepoint or the endpoint with its children
		do
		{
			bool shareBaseVertex = this->getBase() == candidate->getBase();
			bool shareEndVertex = this->getNext()->getBase() == candidate->getNext()->getBase();

			if (shareBaseVertex || shareEndVertex)
			{
				fatherHEdge = candidate;
				break;
			}

			// try with the next half-edge on the father face
			candidate = candidate->getNext();
		} while (candidate != this->getFace()->getFather()->getRepresentative());

	}

	return fatherHEdge;
}

void UHalfEdge::ApplyEndpointMarking()
{
	if (this->getEnd()->isMarked())
	{
		this->ChangeColor();
		this->getNext()->ChangeColor();
	}
}
