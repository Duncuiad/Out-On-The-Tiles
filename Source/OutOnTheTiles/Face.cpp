// Fill out your copyright notice in the Description page of Project Settings.


#include "Face.h"

unsigned int UFace::Depth() const
{
	return this->father ? 1 + this->father->Depth() : 1;
}

int UFace::FaceType() const
{
	/**
	* Recall that triangles and red-blue squares have 2 blue half-edges, while red squares have none
	*/
	int blueSum = 0;
	bool isTriangle = false;
	UHalfEdge* currentHE = this->rep;

	do
	{
		blueSum += currentHE->isBlue();
		isTriangle = !isTriangle;
		currentHE = currentHE->getNext();
	}
	while (currentHE != this->rep);

	return isTriangle ? -blueSum/2 : blueSum/2;
}

FMatrix2x2 UFace::getTransform() const
{
	// find the position of the face's second vertex
	FVector2D edgeVector = this->rep->getNext()->getBase()->getPosition();

	// subtract the position of the first vertex to find the displacement vector
	edgeVector -= this->getPosition();

	// return the similarity transform matrix which sends the vector (1, 0) to edgeVector
	return FMatrix2x2(edgeVector.X, -edgeVector.Y, edgeVector.Y, edgeVector.X);
}
