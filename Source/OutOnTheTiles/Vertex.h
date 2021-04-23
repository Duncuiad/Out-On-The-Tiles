// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Vector2D.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Vertex.generated.h"


/**
 * A class for the vertices in the tiling
 * It stores their position and their color (mark), which is needed when subdividing the tiling
 */
UCLASS()
class OUTONTHETILES_API UVertex : public UObject
{
	GENERATED_BODY()

	/**
	 * Stores information to determine what kind of subdivision to apply to the mesh
	 */
	bool marked;

public:
	FVector2D position;

	/**
	 * Get and set for marked vertex information
	 */
	inline bool isMarked() const { return this->marked; }
	inline void Mark() { this->marked = true; }
	inline void Unmark() { this->marked = false; }

	/**
	 * Get and set for vertex position
	 */
	inline FVector2D getPosition() const { return this->position; }
	inline void setPosition(float newX, float newY) { this->position = FVector2D(newX, newY); }
	inline void setPosition(const FVector2D& newPosition) { this->position = FVector2D(newPosition); }
	

};
