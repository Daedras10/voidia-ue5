// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/HeapElement.h"

// Add default functionality here for any IHeapElement functions that are not pure virtual.
void IHeapElement::SetHeapIndex(int NewIndex)
{
	HeapIndex = NewIndex;
}

int IHeapElement::GetHeapIndex() const
{
	return HeapIndex;
}

int IHeapElement::HeapCompareTo(const IHeapElement& Other) const
{
	// Greater priority : 1
	// Equal priority : 0
	// Lower priority : -1
	
	return 0;
}
