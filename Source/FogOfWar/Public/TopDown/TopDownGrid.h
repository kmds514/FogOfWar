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
	/** Sets default values for this actor's properties */
	ATopDownGrid();

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	/** @return Returns grid coords index */
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FIntPoint WorldToGrid(const FVector& WorldLocation) const;
	
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
	UFUNCTION()
	void UpdateGridTransform();

	/** @return returns blocking hit */
	UFUNCTION()
	bool CoordsLineTraceToMinusZAxis(const FIntPoint& Coords, FHitResult& OutHit);

	UFUNCTION()
	void GenerateTileData();

	UFUNCTION()
	void DrawDebugGrid();

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugGrid = false;

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugLineTrace = false;

	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 4, ClampMax = 512, UIMin = 4, UIMax = 512))
	int GridResolution = 128;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentXY = 1024;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentZ = 1024;

	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 10, ClampMax = 1000, UIMin = 10, UIMax = 1000))
	int HeightDivisor = 200.0f;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBillboardComponent* Billboard = nullptr;

	UPROPERTY(Category = "Top Down Grid", VisibleAnywhere)
	class UBoxComponent* GridVolume = nullptr;

	/** 그리드 좌표의 최솟값을 (0, 0)으로 하기 위한 조정값 */
	UPROPERTY()
	int GridShift = 64;
};
