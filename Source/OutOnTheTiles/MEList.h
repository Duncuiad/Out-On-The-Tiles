// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

#include "Containers/List.h"

#include "CoreMinimal.h"

/**
 * Mesh Element List, used to store and operate on collections of Vertices, Half-Edges, Faces (one kind at a time)
 */
template<class T>
class OUTONTHETILES_API MEList
{
	TDoubleLinkedList<T> list;

public:
	MEList() : list() {}
	~MEList() {}

private:

	class MEIterator {

		/*
		* Dependent names (e. g. TDoubleLinkedList<T>::TDoubleLinkedListNode)
		* must be preceded by "typename" to be a type
		*/
		typedef typename TDoubleLinkedList<T>::TDoubleLinkedListNode MENode;

		// The iterator points to a node of the list it is built by
		MENode* ptr;

	public:

		typedef MEIterator iterator;
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef T* pointer;
		typedef T& reference;

		MEIterator() : ptr() {}
		MEIterator(const MEIterator& other) : ptr(other.ptr) {}

		inline iterator& operator=(const iterator& other) {
			ptr = other.ptr;
			return *this;
		}

		~MEIterator() {}

		// Returns data contained in the pointed element (dereferencing)
		inline reference operator*() const {
			return ptr->GetValue();
		}

		// Returns the pointer to the data referenced by the iterator
		inline pointer operator->() const {
			return &ptr->GetValue();
		}

		// Post-increment iteration operator
		inline iterator operator++(int) {
			iterator tmp(*this);
			ptr = ptr->GetNextNode();
			return tmp;
		}

		// Pre-increment iteration operator
		inline iterator& operator++() {
			ptr = ptr->GetNextNode();
			return *this;
		}

		// Equality
		bool operator==(const iterator& other) const {
			return (ptr == other.ptr);
		}

		// Unequality
		bool operator!=(const iterator& other) const {
			return (ptr != other.ptr);
		}

	private:

		/*
		The container class needs to be set friend of the iterator
		to be able to use the initialization constructor
		*/
		friend class MEList;

		/*
		Private initialization constructor to be used by the container class,
		typically in the begin() and end() methods
		*/
		MEIterator(MENode* p) : ptr(p) { }

	};

public:

	// Returns an iterator to the beginning of the list
	inline MEIterator begin() const
	{
		return MEIterator(this->GetHead());
	}

	// Returns an iterator past the end of the list
	inline MEIterator end() const
	{
		return MEIterator(this->GetTail()->GetNextNode());
	}

	// Appends an element at the end of the list
	inline void Append(const T& element)
	{
		list.AddTail(element);
	}

	// Appends otherList to the end of this list
	inline void Append(const MEList& otherList)
	{
		otherList.AddHead(list.GetTail());
		list.AddTail(otherList.GetHead());
	}

	// Removes all elements from the list
	inline void Empty()
	{
		list.Empty();
	}

	// True if the list is empty
	inline bool IsEmpty() const
	{
		// Head is nullptr, if list is empty
		return !((bool) this->list.GetHead());
	}
};
