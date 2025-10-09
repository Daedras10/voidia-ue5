// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeapElement.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHeapElement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOIDIA_API IHeapElement
{
	GENERATED_BODY()

public:
	void SetHeapIndex(int NewIndex);
	int GetHeapIndex() const;

	virtual int HeapCompareTo(const IHeapElement& Other) const;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	int HeapIndex = -1;
};
