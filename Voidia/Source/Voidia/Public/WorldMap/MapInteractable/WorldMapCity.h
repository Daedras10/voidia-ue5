// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap/MapInteractable/MapInteractable.h"
#include "WorldMapCity.generated.h"

/**
 * 
 */
UCLASS()
class VOIDIA_API AWorldMapCity : public AMapInteractable
{
	GENERATED_BODY()

public:
	void SetAsCity();
	void SetAsVillage();

	
protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_SetAsCity();
	UFUNCTION(BlueprintImplementableEvent) void BP_SetAsVillage();
	
};
