// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WorldMap/WorldMapComponent.h"

#include "WorldMap/WorldMap.h"

// Sets default values for this component's properties
UWorldMapComponent::UWorldMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWorldMapComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	OwningWorldMap = Cast<AWorldMap>(GetOwner());
}

