// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapInteractable/MapInteractable.h"

#include "PaperTileLayer.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMap/WorldMap.h"
#include "PaperTileMap.h"

// Sets default values
AMapInteractable::AMapInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeRotation(FRotator(180.f, 180.f, -90.f));
}

void AMapInteractable::SetTileLocation(const int X, const int Y)
{
	CurrentX = X;
	CurrentY = Y;
	MoveTo(X, Y);
}

void AMapInteractable::MoveTo(int X, int Y)
{
	constexpr auto ZOffset = 5.f;
	
	const auto TileMapComponent = WorldMapRef->GetTileMapComponent();
	const int MapWidth = TileMapComponent->TileMap->MapWidth;
	const int MapHeight = TileMapComponent->TileMap->MapHeight;
	if (X < 0 || X >= MapWidth || Y < 0 || Y >= MapHeight) return;

	const int TileWidth = TileMapComponent->TileMap->TileWidth;
	const int TileHeight = TileMapComponent->TileMap->TileHeight;

	const auto WMLocation = TileMapComponent->GetComponentLocation();
	FVector NewLocation = WMLocation + FVector(0.f, 0.f, ZOffset);

	const float XScale = TileMapComponent->GetComponentScale().X;
	const float YScale = TileMapComponent->GetComponentScale().Y;
	
	NewLocation -= FVector(Y * TileHeight * YScale, X * TileWidth * XScale, 0.f);
	
	SetActorLocation(NewLocation);
}

void AMapInteractable::MoveToDebug(int X, int Y)
{
	MoveTo(X, Y);
	FPaperTileInfo TileInfo = FPaperTileInfo();
	WorldMapRef->SetTile(FIntVector2(X, Y), 0, TileInfo);
	WorldMapRef->SetTile(FIntVector2(X, Y), 1, TileInfo);
	
}

void AMapInteractable::MoveToDebugExec()
{
	MoveToDebug(CurrentX, CurrentY);
}

// Called when the game starts or when spawned
void AMapInteractable::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldMap::StaticClass(), FoundActors);
	if (FoundActors.Num() <= 0) return;
	WorldMapRef = Cast<AWorldMap>(FoundActors[0]);
}
