// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/List.h"

#include "CoreMinimal.h"

/**
 * 
 */
template<class T>
class OUTONTHETILES_API MEList
{
	TDoubleLinkedList<T> list;

public:
	MEList() : list() {}
	~MEList();

private:

	class MEIterator {

		T* ptr;

		friend class MEList;

	public:
		MEIterator() : ptr(0) {}
		~MEIterator() {}

	};

public:

	MEIterator begin();
	MEIterator end();
};
