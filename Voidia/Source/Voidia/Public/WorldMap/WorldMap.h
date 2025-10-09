// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"
#include "GameFramework/Actor.h"
#include "Structs/WorldMap/WorldGenerationStructs.h"
#include "WorldMap.generated.h"

class UPaperTileMap;
class UPaperTileMapComponent;
class UWorldMapGenerationComponent;

UCLASS()
class VOIDIA_API AWorldMap : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldMap();

	void SetTileOnMap(FIntVector2 TileCoordinate, UPaperTileMapComponent* TileMapRef, int Layer, FPaperTileInfo TileInfo);
	void SetTile(FIntVector2 TileCoordinate, int Layer, FPaperTileInfo TileInfo);

	UPaperTileMapComponent* GetTileMapComponent() const;
	TArray<TArray<FWorldMapTileInfo>>* GetWorldMapTiles();

	void ValidateMap() const;

protected:
	virtual void BeginPlay() override;



protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPaperTileMapComponent* TileMapComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UWorldMapGenerationComponent* WorldMapGenerationComponent = nullptr;


	UPaperTileMap* TileMap = nullptr;
	TArray<TArray<FWorldMapTileInfo>> WorldMapTiles = TArray<TArray<FWorldMapTileInfo>>();
};
