// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "MapInteractable.generated.h"

class AWorldMap;

UCLASS()
class VOIDIA_API AMapInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapInteractable();

	void SetTileLocation(const int X, const int Y);

	void MoveTo(int X, int Y);
	void MoveToDebug(int X, int Y);
	UFUNCTION(CallInEditor) void MoveToDebugExec();
	

protected:
	virtual void BeginPlay() override;

	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) UPaperSpriteComponent* SpriteComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere) int CurrentX = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) int CurrentY = 0;
	
	AWorldMap* WorldMapRef = nullptr;
};
