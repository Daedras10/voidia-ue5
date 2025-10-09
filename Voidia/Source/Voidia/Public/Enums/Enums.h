// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



UENUM(Blueprintable)
enum class ETileListType : uint8
{
	TLT_None			UMETA(DisplayName = "None"),
	TLT_Water			UMETA(DisplayName = "Water"),
	TLT_Grass			UMETA(DisplayName = "Grass"),
	TLT_Mountain		UMETA(DisplayName = "Mountain"),
	TLT_Sand			UMETA(DisplayName = "Sand"),
	TLT_Swamp			UMETA(DisplayName = "Swamp"),
	TLT_Forest			UMETA(DisplayName = "Forest"),
};

UENUM(Blueprintable)
enum class ETileBaseType : uint8
{
	TBT_None			UMETA(DisplayName = "None"),
	TBT_Water			UMETA(DisplayName = "Water"),
	TBT_Grass			UMETA(DisplayName = "Grass"),
	TBT_Mountain		UMETA(DisplayName = "Mountain"),
	TBT_Sand			UMETA(DisplayName = "Sand"),
	TBT_Swamp			UMETA(DisplayName = "Swamp"),
	TBT_Forest			UMETA(DisplayName = "Forest"),
	TBT_Snow			UMETA(DisplayName = "Snow"),
};

UENUM(Blueprintable)
enum class ETileLayer : uint8
{
	TL_Floor			UMETA(DisplayName = "Background"),
	TL_OverFloor		UMETA(DisplayName = "Base"),
	//TL_Overlay		UMETA(DisplayName = "Overlay"),
	//TL_Props		UMETA(DisplayName = "Props"),
	//TL_AboveProps	UMETA(DisplayName = "Above Props"),
};
