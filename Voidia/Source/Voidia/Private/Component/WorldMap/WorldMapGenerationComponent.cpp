// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WorldMap/WorldMapGenerationComponent.h"

#include "PaperTileLayer.h"
#include "WorldMap/WorldMap.h"
#include "DataAsset/WorldMap/TileInfoDataAsset.h"
#include "WorldMap/MapInteractable/WorldMapCity.h"

// Sets default values for this component's properties
UWorldMapGenerationComponent::UWorldMapGenerationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWorldMapGenerationComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!OwningWorldMap) return;

	if (!TileInfoDataAsset)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset is null"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset is null"));
		return;
	}
	
	//TODO OwningWorldMap
	const auto TileComponent = OwningWorldMap->GetTileMapComponent();
	if (!TileComponent) return;
	
	CreateTileMap(TileComponent);
	GenerateBaseGrassLayer(TileComponent);
	PopulateOceans(TileComponent);
	PopulateForests(TileComponent);
	PopulateMountains(TileComponent);
	PlaceCities(TileComponent);

	if (!bInstantDraw) DrawMap(TileComponent);
	OwningWorldMap->ValidateMap();

	// 0 0 is TOP right (for some reason XD)
	//TileComponent->SetTile(0, 0, 0, FPaperTileInfo());
	

	// /* Clear the map first */
	// FPaperTileInfo TileInfo;
	// for (int X = 0; X < MapWidth; X++)
	// {
	// 	for (int Y = 0; Y < MapHeight; Y++)
	// 	{
	// 		TileInfo.TileSet = nullptr;
	// 		TileInfo.PackedTileIndex = 0;
	// 		TileComponent->SetTile(X, Y, 0, TileInfo);
	// 	}
	// }


	/* Fill with default (water) */
	// auto Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Water);
	// if (!Info)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Water"));
	// 	UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Water"));
	// 	return;
	// }
	// TileInfo.TileSet = Info->TileSet;
	// TileInfo.PackedTileIndex = Info->TileID;
	//
	// for (int X = 0; X < MapWidth; X++)
	// {
	// 	for (int Y = 0; Y < MapHeight; Y++)
	// 	{
	// 		TileComponent->SetTile(X, Y, 0, TileInfo);
	// 	}
	// }
	//
	// Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Grass);
	// if (!Info) return;
	// TileInfo.TileSet = Info->TileSet;
	// TileInfo.PackedTileIndex = Info->TileID;
	// TileComponent->SetTile(0, 0, 0, TileInfo);
}

void UWorldMapGenerationComponent::CreateTileMap(UPaperTileMapComponent* TileMapRef) const
{
	TileMapRef->CreateNewTileMap(MapWidth, MapHeight, TileSize, TileSize, 1);
	TileMapRef->AddNewLayer();
	auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	for (int X = 0; X < MapWidth; X++)
	{
		TArray<FWorldMapTileInfo> Column;
		for (int Y = 0; Y < MapHeight; Y++)
		{
			FWorldMapTileInfo Info;
			Info.TileCoordinate = FIntVector2(X, Y);
			Info.TileTypeByLayers.Empty();
			Info.TileTypeByLayers.Add(ETileLayer::TL_Floor, ETileBaseType::TBT_None);
			Info.TileTypeByLayers.Add(ETileLayer::TL_OverFloor, ETileBaseType::TBT_None);
			Column.Add(Info);
		}
		WorldMapTiles->Add(Column);
	}
}

void UWorldMapGenerationComponent::GenerateBaseGrassLayer(UPaperTileMapComponent* TileMapRef) const
{
	const auto Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Grass);
	if (!Info)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Grass"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Grass"));
		return;
	}

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = Info->TileSet;
	TileInfo.PackedTileIndex = Info->TileID;
	const auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	
	for (int X = 0; X < MapWidth; X++)
	{
		for (int Y = 0; Y < MapHeight; Y++)
		{
			TileMapRef->SetTile(X, Y, 0, TileInfo);
			const auto Tile = &(*WorldMapTiles)[X][Y];
			Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Grass;
		}
	}
}

void UWorldMapGenerationComponent::PopulateOceans(UPaperTileMapComponent* TileMapRef)
{

	/* Water borders */
	const auto Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Water);
	if (!Info)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Water"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Water"));
		return;
	}

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = Info->TileSet;
	TileInfo.PackedTileIndex = Info->TileID;
	const auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();

	for (int X = 0; X < MapWidth; X++)
	{
		int OceanWidth = FMath::RandRange(MinOceanWidth, MaxOceanWidth);
		for (int Y = 0; Y < OceanWidth; Y++)
		{
			TileMapRef->SetTile(X, Y, 0, TileInfo);
			const auto Tile = &(*WorldMapTiles)[X][Y];
			Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
		}

		OceanWidth = FMath::RandRange(MinOceanWidth, MaxOceanWidth);
		for (int Y = MapHeight - 1; Y > MapHeight - 1 - OceanWidth; Y--)
		{
			TileMapRef->SetTile(X, Y, 0, TileInfo);
			const auto Tile = &(*WorldMapTiles)[X][Y];
			Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
		}
	}

	for (int Y = 0; Y < MapHeight; Y++)
	{
		int OceanWidth = FMath::RandRange(MinOceanWidth, MaxOceanWidth);
		for (int X = 0; X < OceanWidth; X++)
		{
			TileMapRef->SetTile(X, Y, 0, TileInfo);
			const auto Tile = &(*WorldMapTiles)[X][Y];
			Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
		}

		OceanWidth = FMath::RandRange(MinOceanWidth, MaxOceanWidth);
		for (int X = MapWidth - 1; X > MapWidth - 1 - OceanWidth; X--)
		{
			TileMapRef->SetTile(X, Y, 0, TileInfo);
			const auto Tile = &(*WorldMapTiles)[X][Y];
			Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
		}
	}


	/* Random Ocean propagation */
	const int PropagationIterations = FMath::RandRange(MinOceanPropagation, MaxOceanPropagation);

	for (int Current = 0; Current < PropagationIterations; Current++)
	{
		const int Direction = FMath::RandRange(0, 3);
		 // 0 = Up, 1 = Down, 2 = Left, 3 = Right

		int StartX = -1;
		int StartY = -1;

		switch (Direction)
		{
			case 0: // Up
				StartX = FMath::RandRange(0, MapWidth - 1);
				StartY = MapHeight - 1 - MinOceanWidth;
				break;
			case 1: // Down
				StartX = FMath::RandRange(0, MapWidth - 1);
				StartY = MinOceanWidth;
				break;
			case 2: // Left
				StartX = MinOceanWidth;
				StartY = FMath::RandRange(0, MapHeight - 1);
				break;
			case 3: // Right
				StartX = MapWidth - 1 - MinOceanWidth;
				StartY = FMath::RandRange(0, MapHeight - 1);
				break;
			default: break;
		}
		if (StartX == -1 || StartY == -1) continue;

		int Power = FMath::RandRange(6, 15);
		for (int X = StartX - Power; X <= StartX + Power; X++)
		{
			for (int Y = StartY - Power; Y <= StartY + Power; Y++)
			{
				if (X < 0 || X >= MapWidth || Y < 0 || Y >= MapHeight) continue;

				const int DistX = FMath::Abs(X - StartX);
				const int DistY = FMath::Abs(Y - StartY);
				const int Distance = DistX + DistY;
				if (Distance > Power) continue;
				if (Power - DistX < 3 || Power - DistY < 3) continue;

				const auto Tile = &(*WorldMapTiles)[X][Y];
				if (Tile->TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_Water) continue;

				int Chance = FMath::RandRange(0, 100);
				if (Chance < (15 + Distance * 0.5) ) continue;

				TileMapRef->SetTile(X, Y, 0, TileInfo);
				Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
			}
		}
	}



	/* Random lakes */
	TMap<ETileLayer, TArray<ETileBaseType>> ReplacementBiomesForWater;
	ReplacementBiomesForWater.Add(ETileLayer::TL_Floor, {ETileBaseType::TBT_None, ETileBaseType::TBT_Grass});
	
	const int SmallLakeCount = FMath::RandRange(MinSmallLakeCount, MaxSmallLakeCount);
	const int BigLakeCount = FMath::RandRange(MinLargeLakeCount, MaxLargeLakeCount);
	
	for (int Current = 0; Current < SmallLakeCount + BigLakeCount; Current++)
	{
		if (Current < SmallLakeCount)
		{
			RandomPropagateBiome(TileMapRef,
				MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1, MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1,
				FMath::RandRange(MinSmallLakeSize, MaxSmallLakeSize),
				FIntVector2(0, 0),
				ETileBaseType::TBT_Water,
				ETileLayer::TL_Floor,
				ReplacementBiomesForWater,
				TileInfo);
		}
		else
		{
			RandomPropagateBiome(TileMapRef,
				MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1, MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1,
				FMath::RandRange(MinLargeLakeSize, MaxLargeLakeSize),
				FIntVector2(0, 0),
				ETileBaseType::TBT_Water,
				ETileLayer::TL_Floor,
				ReplacementBiomesForWater,
				TileInfo);
		}
	}
	

	/* Clean tiles where at least 3 direct links are water */
	bool ShouldRetry = true;
	TArray<FTileToDraw> Tiles = {};
	while (ShouldRetry)
	{
		ShouldRetry = false;
		for (int X = 0; X < MapWidth; X++)
		{
			for (int Y = 0; Y < MapHeight; Y++)
			{
				const auto Tile = &(*WorldMapTiles)[X][Y];
				const auto TileType = Tile->TileTypeByLayers[ETileLayer::TL_Floor];
				if (TileType == ETileBaseType::TBT_Water) continue;

				int WaterCount = 0;
				// Up
				if (Y < MapHeight - 1)
				{
					const auto UpTile = &(*WorldMapTiles)[X][Y + 1];
					if (UpTile->TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_Water) WaterCount++;
				}
				// Down
				if (Y > 0)
				{
					const auto DownTile = &(*WorldMapTiles)[X][Y - 1];
					if (DownTile->TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_Water) WaterCount++;
				}
				// Left
				if (X > 0)
				{
					const auto LeftTile = &(*WorldMapTiles)[X - 1][Y];
					if (LeftTile->TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_Water) WaterCount++;
				}
				// Right
				if (X < MapWidth - 1)
				{
					const auto RightTile = &(*WorldMapTiles)[X + 1][Y];
					if (RightTile->TileTypeByLayers[ETileLayer::TL_Floor] == ETileBaseType::TBT_Water) WaterCount++;
				}

				if (WaterCount >= 3)
				{
					ShouldRetry = true;
					Tiles.Add({FIntVector2(X, Y), ETileLayer::TL_Floor});
					Tile->TileTypeByLayers[ETileLayer::TL_Floor] = ETileBaseType::TBT_Water;
					if (bInstantDraw) TileMapRef->SetTile(X, Y, 0, TileInfo);
				}
			}
		}
	}
	if (Tiles.Num() > 0) TilesToDrawn.Add(Tiles);
}

void UWorldMapGenerationComponent::PopulateForests(UPaperTileMapComponent* TileMapRef)
{
	const auto Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Forest);
	if (!Info)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Forest"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Forest"));
		return;
	}

	TMap<ETileLayer, TArray<ETileBaseType>> ReplacementBiomesForForest;
	ReplacementBiomesForForest.Add(ETileLayer::TL_Floor, {ETileBaseType::TBT_Grass});
	ReplacementBiomesForForest.Add(ETileLayer::TL_OverFloor, {ETileBaseType::TBT_None});

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = Info->TileSet;
	TileInfo.PackedTileIndex = Info->TileID;

	int MinSpawn = MinOceanWidth*3;
	int MaxSpawnX = MapWidth - (MinOceanWidth*3) - 1;
	int MaxSpawnY = MapHeight - (MinOceanWidth*3) - 1;

	int ForestCount = FMath::RandRange(MinForestCount, MaxForestCount);
	for (int Current = 0; Current < ForestCount; Current++)
	{
		int Random = FMath::RandRange(0, 100);
		if (Random >= BigForestChance || true)
		{
			RandomPropagateBiome(TileMapRef,
				MinSpawn, MaxSpawnX, MinSpawn, MaxSpawnY,
				FMath::RandRange(MinForestSize, MaxForestSize),
				FIntVector2(0, 0),
				ETileBaseType::TBT_Forest,
				ETileLayer::TL_OverFloor,
				ReplacementBiomesForForest,
				TileInfo);
		}
		else
		{
			continue;
			RandomPropagateBiome(TileMapRef,
				MinSpawn, MaxSpawnX, MinSpawn, MaxSpawnY,
				FMath::RandRange(MinForestSize * 3, MaxForestSize * 3),
				FIntVector2(0, 0),
				ETileBaseType::TBT_Forest,
				ETileLayer::TL_OverFloor,
				ReplacementBiomesForForest,
				TileInfo);
		}
	}
}

void UWorldMapGenerationComponent::PopulateMountains(UPaperTileMapComponent* TileMapRef)
{
	const auto Info = TileInfoDataAsset->TileInfoMap.Find(ETileListType::TLT_Forest);
	if (!Info)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Forest"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain TLT_Forest"));
		return;
	}

	TMap<ETileLayer, TArray<ETileBaseType>> ReplacementBiomes;
	ReplacementBiomes.Add(ETileLayer::TL_Floor, {ETileBaseType::TBT_Grass});
	ReplacementBiomes.Add(ETileLayer::TL_OverFloor, {ETileBaseType::TBT_None});

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = Info->TileSet;
	TileInfo.PackedTileIndex = Info->TileID;

	int MinSpawn = MinOceanWidth*3;
	int MaxSpawnX = MapWidth - (MinOceanWidth*3) - 1;
	int MaxSpawnY = MapHeight - (MinOceanWidth*3) - 1;

	int Count = FMath::RandRange(MinMountainCount, MaxMountainCount);
	for (int Current = 0; Current < Count; Current++)
	{
		RandomPropagateBiome(TileMapRef,
			MinSpawn, MaxSpawnX, MinSpawn, MaxSpawnY,
			FMath::RandRange(MinMountainSize, MaxMountainSize),
			FIntVector2(0, 0),
			ETileBaseType::TBT_Mountain,
			ETileLayer::TL_OverFloor,
			ReplacementBiomes,
			TileInfo);
	}
}

void UWorldMapGenerationComponent::PlaceCities(UPaperTileMapComponent* TileMapRef)
{
	if (!CityClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::PlaceCities::CityClass is null"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::PlaceCities::CityClass is null"));
		return;
	}
	
	const auto NumberOfCities = FMath::RandRange(MinCityCount, MaxCityCount);
	auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();

	TArray<FIntVector2> ExistingCityLocations = {};
	TArray<FIntVector2> ExistingVillageLocations = {};
	
	for (int i = 0; i < NumberOfCities; i++)
	{
		int X = FMath::RandRange(MinOceanWidth*3, MapWidth - (MinOceanWidth*3) - 1);
		int Y = FMath::RandRange(MinOceanWidth*3, MapHeight - (MinOceanWidth*3) - 1);

		auto Safety = 0;
		while (!CanPlaceCityAtLocation(FIntVector2(X, Y), ExistingCityLocations) && Safety < 200)
		{
			X = FMath::RandRange(MinOceanWidth*3, MapWidth - (MinOceanWidth*3) - 1);
			Y = FMath::RandRange(MinOceanWidth*3, MapHeight - (MinOceanWidth*3) - 1);
		}
		if (Safety >= 200) continue;
		auto Tile = &(*WorldMapTiles)[X][Y];

		//0,15625 => 16 / 512 = 0,03125 * 5 = 0,15625 (Size / ImageSize) * Scale)
		
		UClass* ClassType = CityClass->GetDefaultObject()->GetClass();
		FActorSpawnParameters SpawnParams;
		const auto Transform = FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector(0.15625f, 0.15625f, 0.15625f));
		AActor* Actor = GetWorld()->SpawnActor(ClassType, &Transform, SpawnParams);
		const auto City = Cast<AWorldMapCity>(Actor);
		if (!City) continue;

		Tile->MapInteractablesOnTile.Add(City);
		ExistingCityLocations.Add(FIntVector2(X, Y));
		City->SetTileLocation(X, Y);

		City->SetAsCity();
	}

	for (const auto ExistingCity : ExistingCityLocations)
	{
		const auto VillageCount = FMath::RandRange(MinVillagePerCity, MaxVillagePerCity);
		for (int i = 0; i < VillageCount; i++)
		{
			int X = FMath::RandRange(ExistingCity.X - VillageDistanceFromCity, ExistingCity.X + VillageDistanceFromCity);
			int Y = FMath::RandRange(ExistingCity.Y - VillageDistanceFromCity, ExistingCity.Y + VillageDistanceFromCity);

			auto Safety = 0;
			while (!CanPlaceVillageAtLocation(FIntVector2(X, Y), ExistingCity, ExistingCityLocations, ExistingVillageLocations) && Safety < 200)
			{
				X = FMath::RandRange(ExistingCity.X - VillageDistanceFromCity, ExistingCity.X + VillageDistanceFromCity);
				Y = FMath::RandRange(ExistingCity.Y - VillageDistanceFromCity, ExistingCity.Y + VillageDistanceFromCity);
			}
			if (Safety >= 200) continue;
			auto Tile = &(*WorldMapTiles)[X][Y];

			UClass* ClassType = CityClass->GetDefaultObject()->GetClass();
			FActorSpawnParameters SpawnParams;
			const auto Transform = FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector(0.15625f, 0.15625f, 0.15625f));
			AActor* Actor = GetWorld()->SpawnActor(ClassType, &Transform, SpawnParams);
			const auto City = Cast<AWorldMapCity>(Actor);
			if (!City) continue;

			Tile->MapInteractablesOnTile.Add(City);
			ExistingVillageLocations.Add(FIntVector2(X, Y));
			City->SetTileLocation(X, Y);

			City->SetAsVillage();
		}
	}
}

void UWorldMapGenerationComponent::DrawMap(UPaperTileMapComponent* TileMapRef)
{
	if (TilesToDrawn.Num() == 0) return;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
    
	//Binding the function with specific values
	TimerDel.BindUFunction(this, FName("DrawNextTileGroup"), TileMapRef);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DrawTimeBetweenEachType, false);
}

void UWorldMapGenerationComponent::DrawTile(UPaperTileMapComponent* TileMapRef, const FIntVector2 TileCoordinate,
	const ETileLayer Layer)
{
	const auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	const auto Tile = &(*WorldMapTiles)[TileCoordinate.X][TileCoordinate.Y];

	auto Type = ETileListType::TLT_None;
	switch (Tile->TileTypeByLayers[Layer])
	{
	case ETileBaseType::TBT_None: Type = ETileListType::TLT_None; break;
	case ETileBaseType::TBT_Grass: Type = ETileListType::TLT_Grass; break;
	case ETileBaseType::TBT_Water: Type = ETileListType::TLT_Water; break;
	case ETileBaseType::TBT_Mountain: Type = ETileListType::TLT_Mountain; break;
	case ETileBaseType::TBT_Sand: Type = ETileListType::TLT_Sand; break;
	case ETileBaseType::TBT_Swamp: Type = ETileListType::TLT_Swamp; break;
	case ETileBaseType::TBT_Forest: Type = ETileListType::TLT_Forest; break;
	case ETileBaseType::TBT_Snow: Type = ETileListType::TLT_None; break;
	default: break;
	}

	const auto Info = TileInfoDataAsset->TileInfoMap.Find(Type);
	if (!Info)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain"));
		UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::BeginPlay::TileInfoDataAsset::TileInfoMap does not contain"));
		return;
	}

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = Info->TileSet;
	TileInfo.PackedTileIndex = Info->TileID;

	int LayerID = 0;
	switch (Layer) {
	case ETileLayer::TL_Floor:
		LayerID = 0;
		break;
	case ETileLayer::TL_OverFloor:
		LayerID = 1;
		break;
	}

	TileMapRef->SetTile(TileCoordinate.X, TileCoordinate.Y, LayerID, TileInfo);
}

void UWorldMapGenerationComponent::DrawNextTileGroup(UPaperTileMapComponent* TileMapRef)
{
	if (TilesToDrawn.Num() == 0) return;
	DrawNextTile(TileMapRef);
}

void UWorldMapGenerationComponent::DrawNextTile(UPaperTileMapComponent* TileMapRef)
{
	if (TilesToDrawn.Num() == 0) return;
	const auto Tiles = TilesToDrawn[0];

	if (Tiles.Num() == 0)
	{
		TilesToDrawn.RemoveAt(0);
		DrawNextTileGroup(TileMapRef);
		return;
	}

	auto TileToDrawn = Tiles[0];
	DrawTile(TileMapRef, TileToDrawn.TileCoordinate, TileToDrawn.Layer);
	TilesToDrawn[0].RemoveAt(0);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

	if (TilesToDrawn[0].Num() == 0)
	{
		TilesToDrawn.RemoveAt(0);
		if (TilesToDrawn.Num() == 0) return;
		TimerDel.BindUFunction(this, FName("DrawNextTileGroup"), TileMapRef);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DrawTimeBetweenEachType, false);
		return;
	}

	TimerDel.BindUFunction(this, FName("DrawNextTile"), TileMapRef);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DrawTimeBetweenEachNode, false);
}

void UWorldMapGenerationComponent::RandomPropagateBiome(UPaperTileMapComponent* TileMapRef, const int MinX,
	const int MaxX, const int MinY, const int MaxY, const int PropagationCount, const FIntVector2 Chance,
	const ETileBaseType BiomeType, const ETileLayer Layer, const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace,
	const FPaperTileInfo& TileInfo)
{
	const auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	
	int StartX = FMath::RandRange(MinX, MaxX);
	int StartY = FMath::RandRange(MinY, MaxY);
	UE_LOG(LogTemp, Warning, TEXT("UWorldMapGenerationComponent::RandomPropagateBiome: RollX: %d [%d, %d], RollY: %d [%d, %d]"), StartX, MinX, MaxX, StartY, MinY, MaxY);

	auto Tile = &(*WorldMapTiles)[StartX][StartY];
	while (!TileValidForReplacement(Tile, BiomesToReplace))
	{
		StartX = FMath::RandRange(MinX, MaxX);
		StartY = FMath::RandRange(MinY, MaxY);
		Tile = &(*WorldMapTiles)[StartX][StartY];
	}

	FIntVector2 StartCoordinate = FIntVector2(StartX, StartY);
	PropagateBiome(TileMapRef, StartCoordinate, PropagationCount, Chance, BiomeType, Layer, BiomesToReplace, TileInfo);
}


void UWorldMapGenerationComponent::PropagateBiome(UPaperTileMapComponent* TileMapRef,
	const FIntVector2 StartCoordinate, const int PropagationCount, const FIntVector2 Chance,
	const ETileBaseType BiomeType, const ETileLayer Layer, const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace,
	const FPaperTileInfo& TileInfo)
{
	const auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();

	const int StartX = StartCoordinate.X;
	const int StartY = StartCoordinate.Y;
	const auto Tile = &(*WorldMapTiles)[StartX][StartY];
	
	int CurrentSize = 1;
	TArray<FTileToDraw> Tiles = {};
	Tile->TileTypeByLayers[Layer] = BiomeType;
	Tiles.Add(FTileToDraw(Tile->TileCoordinate, Layer));
	if (bInstantDraw) DrawTile(TileMapRef, Tile->TileCoordinate, Layer);
	int Safety = 0;

	while (CurrentSize < PropagationCount && Safety < 800)
	{
		Safety++;
		const int Index = FMath::RandRange(0, Tiles.Num() - 1);
		const auto TileCoord = Tiles[Index];

		const int Direction = FMath::RandRange(0, 3);
		int NewX = TileCoord.TileCoordinate.X;
		int NewY = TileCoord.TileCoordinate.Y;

		switch (Direction)
		{
			case 0: // Up
				NewY += 1;
				break;
			case 1: // Down
				NewY -= 1;
				break;
			case 2: // Left
				NewX -= 1;
				break;
			case 3: // Right
				NewX += 1;
				break;
			default: break;
		}

		if (NewX < 0 || NewX >= MapWidth - 1 ||
			NewY < 0 || NewY >= MapHeight - 1) continue;

		auto NewTile = &(*WorldMapTiles)[NewX][NewY];
		if (!TileValidForReplacement(NewTile, BiomesToReplace)) continue;

		NewTile->TileTypeByLayers[Layer] = BiomeType;
		Tiles.Add(FTileToDraw(NewTile->TileCoordinate, Layer));
		if (bInstantDraw) DrawTile(TileMapRef, NewTile->TileCoordinate, Layer);
		CurrentSize++;
		//Safety = 0;
	}

	TilesToDrawn.Add(Tiles);
}

bool UWorldMapGenerationComponent::TileValidForReplacement(const FWorldMapTileInfo* TileInfo,
	const TMap<ETileLayer, TArray<ETileBaseType>> BiomesToReplace) const
{
	for (auto Layer : TileInfo->TileTypeByLayers)
	{
		if (!BiomesToReplace.Contains(Layer.Key)) continue;
		if (BiomesToReplace[Layer.Key].IsEmpty()) continue;
		if (!BiomesToReplace[Layer.Key].Contains(Layer.Value)) return false;
	}
	return true;
}

bool UWorldMapGenerationComponent::CanPlaceCityAtLocation(const FIntVector2 Location,
	const TArray<FIntVector2> ExistingCityLocations) const
{
	auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	const auto Tile = &(*WorldMapTiles)[Location.X][Location.Y];

	TMap<ETileLayer, TArray<ETileBaseType>> ValidBiomesForCity;
	ValidBiomesForCity.Add(ETileLayer::TL_Floor, {ETileBaseType::TBT_Grass});
	ValidBiomesForCity.Add(ETileLayer::TL_OverFloor, {ETileBaseType::TBT_None});
	if (!TileValidForReplacement(Tile, ValidBiomesForCity)) return false;

	for (auto MapInteractable : Tile->MapInteractablesOnTile)
	{
		const auto City = Cast<AWorldMapCity>(MapInteractable);
		if (City) return false;
	}

	for (const auto& ExistingCity : ExistingCityLocations)
	{
		const float Distance2D = FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(ExistingCity.X, ExistingCity.Y));
		if (Distance2D <= CityDistanceApart) return false;
	}

	return true;
}

bool UWorldMapGenerationComponent::CanPlaceVillageAtLocation(const FIntVector2 Location, const FIntVector2 CityLocation, 
	const TArray<FIntVector2> ExistingCityLocations, const TArray<FIntVector2> ExistingVillagesLocations) const
{
	if (Location == CityLocation) return false;
	auto WorldMapTiles = OwningWorldMap->GetWorldMapTiles();
	const auto Tile = &(*WorldMapTiles)[Location.X][Location.Y];

	TMap<ETileLayer, TArray<ETileBaseType>> ValidBiomesForCity;
	ValidBiomesForCity.Add(ETileLayer::TL_Floor, {ETileBaseType::TBT_Grass});
	ValidBiomesForCity.Add(ETileLayer::TL_OverFloor, {ETileBaseType::TBT_None});
	if (!TileValidForReplacement(Tile, ValidBiomesForCity)) return false;

	for (auto MapInteractable : Tile->MapInteractablesOnTile)
	{
		const auto City = Cast<AWorldMapCity>(MapInteractable);
		if (City) return false;
	}

	for (const auto& ExistingCity : ExistingCityLocations)
	{
		if (ExistingCity == CityLocation) continue;
		const float Distance2D = FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(ExistingCity.X, ExistingCity.Y));
		if (Distance2D < VillageDistanceApart) return false;
	}

	for (const auto& ExistingVillage : ExistingVillagesLocations)
	{
		const float Distance2D = FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(ExistingVillage.X, ExistingVillage.Y));
		if (Distance2D < VillageDistanceApart) return false;
	}

	const float DistanceToCity = FVector2D::Distance(FVector2D(Location.X, Location.Y), FVector2D(CityLocation.X, CityLocation.Y));
	if (DistanceToCity > VillageDistanceFromCity) return false;
	if (DistanceToCity < 2) return false;

	return true;
}

/*
// const int LakeSize = (Current < SmallLakeCount) ?
		// 	FMath::RandRange(MinSmallLakeSize, MaxSmallLakeSize) :
		// 	FMath::RandRange(MinLargeLakeSize, MaxLargeLakeSize);
		//
		//
		// int StartX = FMath::RandRange(MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1);
		// int StartY = FMath::RandRange(MinOceanWidth * 2, MapHeight - (MinOceanWidth * 2) - 1);
		//
		// auto Tile = &(*WorldMapTiles)[StartX][StartY];
		// if (Tile->TileType == ETileListType::TLT_Water)
		// {
		// 	while (Tile->TileType == ETileListType::TLT_Water)
		// 	{
		// 		StartX = FMath::RandRange(MinOceanWidth * 2, MapWidth - (MinOceanWidth * 2) - 1);
		// 		StartY = FMath::RandRange(MinOceanWidth * 2, MapHeight - (MinOceanWidth * 2) - 1);
		// 		Tile = &(*WorldMapTiles)[StartX][StartY];
		// 	}
		// }
		//
		// int CurrentLakeSize = 1;
		// TArray<FIntVector2> LakeTiles = {};
		// Tile = &(*WorldMapTiles)[StartX][StartY];
		// Tile->TileType = ETileListType::TLT_Water;
		// TileMapRef->SetTile(StartX, StartY, 0, TileInfo);
		// LakeTiles.Add(Tile->TileCoordinate);
		// int Safety = 0;
		//
		// while (CurrentLakeSize < LakeSize && Safety < 500)
		// {
		// 	Safety++;
		// 	const int Index = FMath::RandRange(0, LakeTiles.Num() - 1);
		// 	const auto LakeTile = LakeTiles[Index];
		//
		// 	const int Direction = FMath::RandRange(0, 3);
		// 	int NewX = LakeTile.X;
		// 	int NewY = LakeTile.Y;
		//
		// 	switch (Direction)
		// 	{
		// 		case 0: // Up
		// 			NewY += 1;
		// 			break;
		// 		case 1: // Down
		// 			NewY -= 1;
		// 			break;
		// 		case 2: // Left
		// 			NewX -= 1;
		// 			break;
		// 		case 3: // Right
		// 			NewX += 1;
		// 			break;
		// 		default: break;
		// 	}
		//
		// 	if (NewX < MinOceanWidth || NewX >= MapWidth - MinOceanWidth ||
		// 		NewY < MinOceanWidth || NewY >= MapHeight - MinOceanWidth) continue;
		//
		// 	auto NewTile = &(*WorldMapTiles)[NewX][NewY];
		// 	if (NewTile->TileType == ETileListType::TLT_Water) continue;
		//
		// 	NewTile->TileType = ETileListType::TLT_Water;
		// 	TileMapRef->SetTile(NewX, NewY, 0, TileInfo);
		// 	LakeTiles.Add(NewTile->TileCoordinate);
		// 	CurrentLakeSize++;
		// 	Safety = 0;
		// }
*/
