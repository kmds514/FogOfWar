// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownGrid.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopDownGrid();

	/**
	 * Called when an instance of this class is placed (in editor) or spawned.
	 * @param	Transform			The transform the actor was constructed at.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FIntPoint WorldToGrid(const FVector& WorldLocation);

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GridToWorld(const FIntPoint& GridCoords);

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GetTileExtent() const;

protected:
	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	FTransform GridTransform;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	FVector TileExtent;

private:
	void UpdateGridTransform();
	void DrawDebugTile();

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugGrid = false;

	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 4, ClampMax = 256, UIMin = 4, UIMax = 256))
	int GridResolution = 128;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeScaleXY = 1024;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeScaleZ = 1024;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBillboardComponent* Billboard = nullptr;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBoxComponent* GridVolume = nullptr;

	/** Bias value for counting in first quadrant */
	UPROPERTY()
	int GridShift = 64;
};
