// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/Enums.h"
#include "Structs/WorldMap/WorldGenerationStructs.h"
#include "TileInfoDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VOIDIA_API UTileInfoDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Info")
	TMap<ETileListType, FTileSourceInfo> TileInfoMap = {};
};
