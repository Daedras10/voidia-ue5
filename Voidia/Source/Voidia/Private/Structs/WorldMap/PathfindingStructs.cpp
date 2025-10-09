// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/WorldMap/PathfindingStructs.h"

float FPathNode::GetFCost() const
{
	return GCost + HCost;
}

float FPathNode::GetGCost() const
{
	return GCost;
}

float FPathNode::GetHCost() const
{
	return HCost;
}

bool FPathNode::IsWalkable() const
{
	return true;
}

float FPathNode::GetDistanceToNode(const FPathNode& Other) const
{
	return FVector2D::Distance(FVector2D(X, Y), FVector2D(Other.X, Other.Y));
}

bool FPathNode::operator==(const FPathNode& Other) const
{
	return X == Other.X && Y == Other.Y;
}

int FPathNode::HeapCompareTo(const IHeapElement& Other) const
{
	const auto OtherNode = static_cast<const FPathNode&>(Other);
	const int SelfFCost = GetFCost();
	const int OtherFCost = OtherNode.GetFCost();
	int Compare = SelfFCost - OtherFCost;
	if (Compare == 0) Compare = HCost - OtherNode.HCost;
	return -Compare;
}

// template <typename T>
// void TFHeap<T>::Add(T NewElement)
// {
// 	auto NewValue = get_value(NewElement);
// 	if (!NewValue) return;
// 	NewValue->SetHeapIndex(CurrentCount);
// 	Elements[CurrentCount] = NewElement;
// 	SortUp(NewElement);
// 	CurrentCount++;
// }
//
// template <typename T>
// T TFHeap<T>::RemoveFirst()
// {
// 	T FirstElement = Elements[0];
// 	CurrentCount--;
// 	Elements[0] = Elements[CurrentCount];
// 	get_value(Elements[0])->SetHeapIndex(0);
// 	SortDown(Elements[0]);
// 	return FirstElement;
// }
//
// template <typename T>
// bool TFHeap<T>::IsEmpty() const
// {
// 	return CurrentCount == 0;
// }
//
// template <typename T>
// bool TFHeap<T>::Contains(T Element) const
// {
// 	auto ElementValue = get_value(Element);
// 	auto ElementCompare = get_value(Elements[ElementValue->GetHeapIndex()]);
// 	return ElementValue == ElementCompare;
//
// 	/*
// 	 * better version for garbage
// 	public bool Contains(T item)
// {
// 	if (item.HeapIndex < currentItemCount)
// 	{
// 		return Equals(items[item.HeapIndex], item);
// 	} else
// 	{
// 		return false;
// 	}
// }
// 	 */
// }
//
// template <typename T>
// int TFHeap<T>::GetCount() const
// {
// 	return CurrentCount;
// }
//
// template <typename T>
// void TFHeap<T>::UpdateElement(T Element, bool bSortUp, bool bSortDown)
// {
// 	if (bSortUp) SortUp(Element);
// 	if (bSortDown) SortDown(Element);
// }
//
// template <typename T>
// void TFHeap<T>::SortUp(T Element)
// {
// 	auto ParentValue = get_value(Element);
// 	if (!ParentValue) return;
// 	int ParentIndex = (ParentValue->GetHeapIndex() - 1) / 2;
// 	while (true)
// 	{
// 		T ParentElement = Elements[ParentIndex];
// 		auto ElementValue = get_value(Element);
// 		if (!ElementValue) break;
// 		if (ParentValue->HeapCompareTo(*ElementValue) >= 0) break;
// 		this->Swap(Element, ParentElement);
// 		ParentIndex = (ElementValue->GetHeapIndex() - 1) / 2;
// 		ParentValue = get_value(Element);
// 		if (!ParentValue) break;
// 	}
// }
//
// template <typename T>
// void TFHeap<T>::SortDown(T Element)
// {
// 	while (true)
// 	{
// 		int ChildIndexLeft = get_value(Element)->GetHeapIndex() * 2 + 1;
// 		int ChildIndexRight = get_value(Element)->GetHeapIndex() * 2 + 2;
// 		int SwapIndex = -1;
//
// 		if (ChildIndexLeft < CurrentCount)
// 		{
// 			SwapIndex = ChildIndexLeft;
//
// 			if (ChildIndexRight < CurrentCount)
// 			{
// 				auto LeftValue = get_value(Elements[ChildIndexLeft]);
// 				auto RightValue = get_value(Elements[ChildIndexRight]);
// 				if (LeftValue && RightValue && LeftValue->HeapCompareTo(*RightValue) < 0) SwapIndex = ChildIndexRight;
// 			}
//
// 			auto ElementValue = get_value(Element);
// 			auto SwapValue = get_value(Elements[SwapIndex]);
// 			if (ElementValue && SwapValue && ElementValue->HeapCompareTo(*SwapValue) < 0)
// 			{
// 				this->Swap(Element, Elements[SwapIndex]);
// 			}
// 			else
// 			{
// 				return;;
// 			}
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}
// }
//
// template <typename T>
// void TFHeap<T>::Swap(T A, T B)
// {
// 	int AIndex = get_value(A)->GetHeapIndex();
// 	int BIndex = get_value(B)->GetHeapIndex();
// 	
// 	Elements[AIndex] = B;
// 	Elements[BIndex] = A;
// 	get_value(A)->SetHeapIndex(BIndex);
// 	get_value(B)->SetHeapIndex(AIndex);
// }
//
// template <typename T>
// void TFHeap<T>::Clear()
// {
// 	CurrentCount = 0;
// }
//
// template <typename T>
// IHeapElement* TFHeap<T>::get_value(const T& Element)
// {
// 	return Element.get();
// }
