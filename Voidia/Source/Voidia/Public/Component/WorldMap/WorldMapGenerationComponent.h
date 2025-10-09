// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "WorldMapComponent.h"
#include "Components/ActorComponent.h"
#include "Enums/Enums.h"
#include "Structs/WorldMap/WorldGenerationStructs.h"
#include "WorldMapGenerationComponent.generated.h"


class AWorldMapCity;
class UPaperTileMapComponent;
class UTileInfoDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDIA_API UWorldMapGenerationComponent : public UWorldMapComponent
{
	GENERATED_BODY()

public:	
	UWorldMapGenerationComponent();


protected:
	virtual void BeginPlay() override;

	void CreateTileMap(UPaperTileMapComponent* TileMapRef) const;
	void GenerateBaseGrassLayer(UPaperTileMapComponent* TileMapRef) const;
	void PopulateOceans(UPaperTileMapComponent* TileMapRef);
	void PopulateForests(UPaperTileMapComponent* TileMapRef);
	void PopulateMountains(UPaperTileMapComponent* TileMapRef);
	void PlaceCities(UPaperTileMapComponent* TileMapRef);

	void DrawMap(UPaperTileMapComponent* TileMapRef);
	void DrawTile(UPaperTileMapComponent* TileMapRef, const FIntVector2 TileCoordinate, const ETileLayer Layer);

	UFUNCTION() void DrawNextTileGroup(UPaperTileMapComponent* TileMapRef);
	UFUNCTION() void DrawNextTile(UPaperTileMapComponent* TileMapRef);
	
	void RandomPropagateBiome(UPaperTileMapComponent* TileMapRef,
		const int MinX, const int MaxX, const int MinY, const int MaxY, const int PropagationCount,
		const FIntVector2 Chance, const ETileBaseType BiomeType, const ETileLayer Layer,
		const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace, //const TArray<ETileBaseType> BiomeItCantOverride,
		const FPaperTileInfo& TileInfo);
	
	void PropagateBiome(UPaperTileMapComponent* TileMapRef,
		const FIntVector2 StartCoordinate, const int PropagationCount,
		const FIntVector2 Chance, const ETileBaseType BiomeType, const ETileLayer Layer,
		const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace, //const TArray<ETileBaseType> BiomeItCantOverride,
		const FPaperTileInfo& TileInfo);

	bool TileValidForReplacement(const FWorldMapTileInfo* TileInfo, const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace) const;
	bool CanPlaceCityAtLocation(const FIntVector2 Location, const TArray<FIntVector2> ExistingCityLocations) const;
	bool CanPlaceVillageAtLocation(const FIntVector2 Location, const FIntVector2 CityLocation, const TArray<FIntVector2> ExistingCityLocations, const TArray<FIntVector2> ExistingVillagesLocations) const;

	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) UTileInfoDataAsset* TileInfoDataAsset = nullptr;

	/* Map Settings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") int MapWidth = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") int MapHeight = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") int TileSize = 16;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") float DrawTimeBetweenEachType = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") float DrawTimeBetweenEachNode = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings") bool bInstantDraw = false;

	
	/* Ocean Settings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Ocean Settings") int MinOceanWidth = 4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Ocean Settings") int MaxOceanWidth = 8;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Ocean Settings") int MinOceanPropagation = 7;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Ocean Settings") int MaxOceanPropagation = 12;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MinSmallLakeCount = 7;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MaxSmallLakeCount = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MinSmallLakeSize = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MaxSmallLakeSize = 21;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MinLargeLakeCount = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MaxLargeLakeCount = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MinLargeLakeSize = 35;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Lake Settings") int MaxLargeLakeSize = 60;


	/* Forest Settings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Forest Settings") int MinForestCount = 7;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Forest Settings") int MaxForestCount = 12;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Forest Settings") int MinForestSize = 15;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Forest Settings") int MaxForestSize = 90;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Forest Settings") int BigForestChance = 20;


	/* Mountain Settings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Mountain Settings") int MinMountainCount = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Mountain Settings") int MaxMountainCount = 7;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Mountain Settings") int MinMountainSize = 35;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|Mountain Settings") int MaxMountainSize = 60;


	/* City Settings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int MinCityCount = 7;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int MaxCityCount = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int CityDistanceApart = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int MinVillagePerCity = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int MaxVillagePerCity = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int VillageDistanceFromCity = 4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") int VillageDistanceApart = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map Settings|City Settings") TSubclassOf<AWorldMapCity> CityClass;

	
	TArray<TArray<FTileToDraw>> TilesToDrawn;
};
