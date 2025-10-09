// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structs/WorldMap/PathfindingStructs.h"
#include "GenericBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VOIDIA_API UGenericBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<FIntVector2> FindPath(const TArray<FPathNode*>& Grid, const FIntVector Size, const FPathNode* StartNode, const FPathNode* EndNode);
	static TArray<FIntVector2> FindPathHeap(const TArray<FPathNode*>& Grid, const FIntVector Size, const FPathNode* StartNode, const FPathNode* EndNode);
};
