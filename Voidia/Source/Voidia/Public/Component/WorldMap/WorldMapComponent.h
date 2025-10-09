// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldMapComponent.generated.h"


class AWorldMap;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDIA_API UWorldMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWorldMapComponent();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) AWorldMap* OwningWorldMap = nullptr;
		
};
