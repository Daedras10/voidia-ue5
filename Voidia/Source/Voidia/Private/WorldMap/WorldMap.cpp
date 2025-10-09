// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/WorldMap.h"
#include "Component/WorldMap/WorldMapGenerationComponent.h"
#include "PaperTileLayer.h"
#include "Structs/WorldMap/WorldGenerationStructs.h"


AWorldMap::AWorldMap()
{
 	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WorldMapGenerationComponent = CreateDefaultSubobject<UWorldMapGenerationComponent>(TEXT("WorldMapGenerationComponent"));
	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));

	TileMapComponent->SetupAttachment(RootComponent);
	TileMapComponent->SetWorldRotation(FRotator(180.f, 180.f, -90.f));
}

void AWorldMap::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	TileMap = TileMapComponent->TileMap;
	if (!TileMap)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TileMap is null"));
		UE_LOG(LogTemp, Warning, TEXT("TileMap is null"));
		return;
	}
}

void AWorldMap::SetTileOnMap(FIntVector2 TileCoordinate, UPaperTileMapComponent* TileMapRef, int Layer,
	FPaperTileInfo TileInfo)
{
	if (!TileMapRef) return;
	TileMapRef->SetTile(TileCoordinate.X, TileCoordinate.Y, Layer, TileInfo);
}

void AWorldMap::SetTile(FIntVector2 TileCoordinate, int Layer, FPaperTileInfo TileInfo)
{
	SetTileOnMap(TileCoordinate, TileMapComponent, Layer, TileInfo);
}

UPaperTileMapComponent* AWorldMap::GetTileMapComponent() const
{
	return TileMapComponent;
}

TArray<TArray<FWorldMapTileInfo>>* AWorldMap::GetWorldMapTiles()
{
	return &WorldMapTiles;
}

void AWorldMap::ValidateMap() const
{
	bool Valid = true;
	for (const auto Line : WorldMapTiles)
	{
		for (const auto Tile : Line)
		{
			if (Tile.TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_None)
			{
				Valid = false;
				UE_LOG(LogTemp, Warning, TEXT("AWorldMap::ValidateMap: Map is not valid! Tile at (%d,%d) is TLT_None"), Tile.TileCoordinate.X, Tile.TileCoordinate.Y);
				break;
			}

			//UE_LOG(LogTemp, Warning, TEXT("AWorldMap::ValidateMap: Tile at (%d,%d) is %d"), Tile.TileCoordinate.X, Tile.TileCoordinate.Y, static_cast<int>(Tile.TileType));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("AWorldMap::ValidateMap: Map is %s"), Valid ? TEXT("valid") : TEXT("not valid"));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, Valid ? FColor::Green : FColor::Red, FString::Printf(TEXT("AWorldMap::ValidateMap: Map is %s"), Valid ? TEXT("valid") : TEXT("not valid")));
}
