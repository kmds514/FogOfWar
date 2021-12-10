// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownGrid.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector WorldLocation;

	UPROPERTY(BlueprintReadWrite)
	int Height;

	UPROPERTY(BlueprintReadWrite)
	bool bVisible = false;

	UPROPERTY(BlueprintReadWrite)
	bool bCanTravel = true;
};

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

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FIntPoint WorldToGrid(const FVector& WorldLocation) const;

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GridToWorld(const FIntPoint& GridCoords) const;
	
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GetTileExtent() const;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	TMap<FIntPoint, FTile> TileData;

protected:
	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	FTransform GridTransform;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	FVector TileExtent;

private:
	void UpdateGridTransform();
	/** @return returns impact hit point */
	FVector CoordsLineTraceToMinusZAxis(const FIntPoint& Coords);
	void GenerateTileData();
	void DrawDebugGrid();

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugGrid = false;

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugLineTrace = false;

	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 4, ClampMax = 256, UIMin = 4, UIMax = 256))
	int GridResolution = 128;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentXY = 1024;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentZ = 1024;

	UPROPERTY(Category = "Config", EditAnywhere)
	float HeightDivisor = 200.0f;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBillboardComponent* Billboard = nullptr;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBoxComponent* GridVolume = nullptr;

	/** 그리드 좌표의 최솟값을 (0, 0)으로 하기 위한 조정값 */
	UPROPERTY()
	int GridShift = 64;
};
