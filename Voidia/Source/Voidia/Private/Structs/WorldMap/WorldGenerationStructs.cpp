// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/WorldMap/WorldGenerationStructs.h"

FWorldMapPathNode::FWorldMapPathNode(const FWorldMapTileInfo* InTileInfo, TArray<TArray<FWorldMapTileInfo>>* InMap) : TileInfo(const_cast<FWorldMapTileInfo*>(InTileInfo))
{
	if (!TileInfo) return;
	X = TileInfo->TileCoordinate.X;
	Y = TileInfo->TileCoordinate.Y;
}
