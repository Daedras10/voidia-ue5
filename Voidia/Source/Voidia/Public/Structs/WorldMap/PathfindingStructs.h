// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HeapElement.h"
//#include "PathfindingStructs.generated.h"


struct FPathNode : public IHeapElement
{
public:
	virtual ~FPathNode() = default;

	virtual float GetFCost() const; /* Total cost */
	virtual float GetGCost() const; /* Cost from start node */
	virtual float GetHCost() const; /* Heuristic cost to end node */
	virtual bool IsWalkable() const;
	virtual float GetDistanceToNode(const FPathNode& Other) const;
	
	virtual bool operator==(const FPathNode& Other) const;

	virtual int HeapCompareTo(const IHeapElement& Other) const override;

	int X = -1;
	int Y = -1;
	
	FPathNode* ParentNode = nullptr;
	TArray<FPathNode*> Neighbors = {};
	

	float GCost = 0.f; // Cost from start node
	float HCost = 0.f; // Heuristic cost to end node

//18:30
};


template<typename T>
struct TFHeap
{
	TFHeap(int Size)
	{
		Elements = TArray<T>();
		Elements.SetNum(Size);
		CurrentCount = 0;
	}

	void Add(T NewElement)
	{
		auto NewValue = get_value(NewElement);
		if (!NewValue) return;
		NewValue->SetHeapIndex(CurrentCount);
		Elements[CurrentCount] = NewElement;
		SortUp(NewElement);
		CurrentCount++;
	}
	
	T RemoveFirst()
	{
		T FirstElement = Elements[0];
		CurrentCount--;
		Elements[0] = Elements[CurrentCount];
		get_value(Elements[0])->SetHeapIndex(0);
		SortDown(Elements[0]);
		return FirstElement;
	}

	bool IsEmpty() const { return CurrentCount == 0; }
	bool Contains(T Element) const
	{
		auto ElementValue = get_value(Element);
		const auto OtherElement = Elements[ElementValue->GetHeapIndex()];
		auto ElementCompare = get_value(OtherElement);
		return ElementValue == ElementCompare;
		/*
	 * better version for garbage
	public bool Contains(T item)
{
	if (item.HeapIndex < currentItemCount)
	{
		return Equals(items[item.HeapIndex], item);
	} else
	{
		return false;
	}
}
	 */
	}
	
	int GetCount() const { return CurrentCount; }

	void UpdateElement(T Element, bool bSortUp = true, bool bSortDown = true) 
	{
		if (bSortUp) SortUp(Element);
		if (bSortDown) SortDown(Element);
	}

	void SortUp(T Element)
	{
		auto ParentValue = get_value(Element);
		if (!ParentValue) return;
		int ParentIndex = (ParentValue->GetHeapIndex() - 1) / 2;
		while (true)
		{
			T ParentElement = Elements[ParentIndex];
			auto ElementValue = get_value(Element);
			if (!ElementValue) break;
			if (ParentValue->HeapCompareTo(*ElementValue) >= 0) break;
			this->Swap(Element, ParentElement);
			ParentIndex = (ElementValue->GetHeapIndex() - 1) / 2;
			ParentValue = get_value(Element);
			if (!ParentValue) break;
		}
	}
	void SortDown(T Element)
	{
		while (true)
		{
			int ChildIndexLeft = get_value(Element)->GetHeapIndex() * 2 + 1;
			int ChildIndexRight = get_value(Element)->GetHeapIndex() * 2 + 2;
			int SwapIndex = -1;

			if (ChildIndexLeft < CurrentCount)
			{
				SwapIndex = ChildIndexLeft;

				if (ChildIndexRight < CurrentCount)
				{
					auto LeftValue = get_value(Elements[ChildIndexLeft]);
					auto RightValue = get_value(Elements[ChildIndexRight]);
					if (LeftValue && RightValue && LeftValue->HeapCompareTo(*RightValue) < 0) SwapIndex = ChildIndexRight;
				}

				auto ElementValue = get_value(Element);
				auto SwapValue = get_value(Elements[SwapIndex]);
				if (ElementValue && SwapValue && ElementValue->HeapCompareTo(*SwapValue) < 0)
				{
					this->Swap(Element, Elements[SwapIndex]);
				}
				else
				{
					return;;
				}
			}
			else
			{
				return;
			}
		}
	}
	
	void Swap(T A, T B)
	{
		int AIndex = get_value(A)->GetHeapIndex();
		int BIndex = get_value(B)->GetHeapIndex();
	
		Elements[AIndex] = B;
		Elements[BIndex] = A;
		get_value(A)->SetHeapIndex(BIndex);
		get_value(B)->SetHeapIndex(AIndex);
	}

	void Clear() { CurrentCount = 0; }


protected:
	IHeapElement* get_value(const T& Element) const { return Cast<IHeapElement>(Element); }

public:
	TArray<T> Elements = TArray<T>();
	int CurrentCount = 0;
};
	
