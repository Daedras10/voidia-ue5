// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfindingStructs.h"
#include "Enums/Enums.h"
#include "WorldGenerationStructs.generated.h"

class AMapInteractable;
class UPaperTileSet;

USTRUCT(Blueprintable)
struct FTileSourceInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TObjectPtr<UPaperTileSet> TileSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int TileID = -1;
};

USTRUCT(Blueprintable)
struct FWorldMapTileInfo
{
	GENERATED_BODY()
public:
	FIntVector2 TileCoordinate = FIntVector2(-1,-1);
	TMap<ETileLayer, ETileBaseType> TileTypeByLayers = {};
	TArray<AMapInteractable*> MapInteractablesOnTile = {};
};

USTRUCT(Blueprintable)
struct FTileToDraw
{
	GENERATED_BODY()
public:
	FIntVector2 TileCoordinate = FIntVector2(-1,-1);
	ETileLayer Layer = ETileLayer::TL_Floor;
};


struct FWorldMapPathNode : public FPathNode
{
public:
	FWorldMapPathNode(const FWorldMapTileInfo* InTileInfo, TArray<TArray<FWorldMapTileInfo>>* InMap);
	
	FWorldMapTileInfo* TileInfo = nullptr;
	/*
	virtual float GetGCost() const;
	virtual float GetHCost() const;
	virtual bool IsWalkable() const;
	virtual float GetDistanceToNode(const FPathNode& Other) const;
	 */
};
