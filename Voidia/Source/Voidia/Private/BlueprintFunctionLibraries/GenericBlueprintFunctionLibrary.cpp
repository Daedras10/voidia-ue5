// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibraries/GenericBlueprintFunctionLibrary.h"

TArray<FIntVector2> UGenericBlueprintFunctionLibrary::FindPath(const TArray<FPathNode*>& Grid, const FIntVector Size,
	const FPathNode* StartNode, const FPathNode* EndNode)
{
	TArray<FPathNode*> OpenList = {};
	TArray<FPathNode*> ClosedList = {};

	OpenList.Add(const_cast<FPathNode*>(StartNode));
	while (OpenList.Num() > 0)
	{
		auto CurrentNode = OpenList[0];
		for (int i = 1; i < OpenList.Num(); i++) // Need to be optimized with a priority queue
		{
			if (OpenList[i]->GetFCost() < CurrentNode->GetFCost() ||
				(OpenList[i]->GetFCost() == CurrentNode->GetFCost() && OpenList[i]->GetHCost() < CurrentNode->GetHCost()))
			{
				CurrentNode = OpenList[i];
			}
		}

		OpenList.Remove(CurrentNode);
		ClosedList.Add(CurrentNode);

		if (*CurrentNode == *EndNode)
		{
			TArray<FIntVector2> Path = {};
			auto TempNode = CurrentNode;
			while (TempNode != nullptr)
			{
				Path.Add(FIntVector2(TempNode->X, TempNode->Y));
				TempNode = TempNode->ParentNode;
			}
			Algo::Reverse(Path);
			return Path;
		}

		for (auto Neighbour : CurrentNode->Neighbors)
		{
			if (!Neighbour->IsWalkable() || ClosedList.Contains(Neighbour)) continue;

			//See how I consider distances
			float NewMovementCostToNeighbour = CurrentNode->GetGCost() + CurrentNode->GetDistanceToNode(*Neighbour);
			if (NewMovementCostToNeighbour < Neighbour->GetGCost() || !OpenList.Contains(Neighbour))
			{
				Neighbour->GCost = NewMovementCostToNeighbour;
				Neighbour->HCost = Neighbour->GetDistanceToNode(*EndNode);
				Neighbour->ParentNode = CurrentNode;

				if (!OpenList.Contains(Neighbour)) OpenList.Add(Neighbour);
			}
		}
	}
	TArray<FIntVector2> EmptyPath = {};
	return EmptyPath; // Return empty path if no path found
	
}

TArray<FIntVector2> UGenericBlueprintFunctionLibrary::FindPathHeap(const TArray<FPathNode*>& Grid,
	const FIntVector Size, const FPathNode* StartNode, const FPathNode* EndNode)
{
	const int MaxNodes = Size.X * Size.Y;

	/* Make theses fixed to avoid Garbage */
	TFHeap<FPathNode*> OpenList = TFHeap<FPathNode*>(MaxNodes);
	TArray<FPathNode*> ClosedList = TArray<FPathNode*>();

	OpenList.Add(const_cast<FPathNode*>(StartNode));
	while (OpenList.GetCount() > 0)
	{
		auto CurrentNode = OpenList.RemoveFirst();
		ClosedList.Add(CurrentNode);
	
		if (*CurrentNode == *EndNode)
		{
			TArray<FIntVector2> Path = {};
			auto TempNode = CurrentNode;
			while (TempNode != nullptr)
			{
				Path.Add(FIntVector2(TempNode->X, TempNode->Y));
				TempNode = TempNode->ParentNode;
			}
			Algo::Reverse(Path);
			return Path;
		}
	
		for (auto Neighbour : CurrentNode->Neighbors)
		{
			if (!Neighbour->IsWalkable() || ClosedList.Contains(Neighbour)) continue;
	
			//See how I consider distances
			float NewMovementCostToNeighbour = CurrentNode->GetGCost() + CurrentNode->GetDistanceToNode(*Neighbour);
			if (NewMovementCostToNeighbour < Neighbour->GetGCost() || !OpenList.Contains(Neighbour))
			{
				Neighbour->GCost = NewMovementCostToNeighbour;
				Neighbour->HCost = Neighbour->GetDistanceToNode(*EndNode);
				Neighbour->ParentNode = CurrentNode;
	
				if (!OpenList.Contains(Neighbour)) OpenList.Add(Neighbour);
			}
		}
	}
	TArray<FIntVector2> EmptyPath = {};
	return EmptyPath; // Return empty path if no path found
}
