// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownGrid.generated.h"

UENUM(BlueprintType)
enum class ETileHeight : uint8
{
	INVALID = 0,
	Lowest,
	Low,
	Medium,
	High,
	Highest
};

USTRUCT(Atomic, BlueprintType)
struct FTile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector WorldLocation;

	UPROPERTY(BlueprintReadWrite)
	ETileHeight Height;
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

protected:
	virtual void BeginPlay() override;

public:
	/** @return Returns grid coords index */
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FIntPoint WorldToGrid(const FVector& WorldLocation) const;
	
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GetTileExtent() const;

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	int GetGridResolution() const;

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	int GetGridVolumeExtentXY() const;
	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	int GetGridVolumeExtentZ() const;

	/** @return Returns world value to grid value */
	int ToGridUnit(const int N) const;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	TMap<FIntPoint, FTile> TileData;

	TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked;

private:
	void UpdateGridTransform();

	/** @return returns blocking hit */
	bool CoordsLineTraceToMinusZAxis(const FIntPoint& Coords, ETraceTypeQuery TraceChannel, FHitResult& OutHit);

	void GenerateTileData();

	void DrawDebugGrid();

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugGrid = false;

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugLineTrace = false;

	/** Number of tiles and Number of fog texel. 
	* Tile extent = GridVolumeExtentXY / GridResoulution */
	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 4, ClampMax = 256, UIMin = 4, UIMax = 256))
	int GridResolution = 128;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentXY = 1024;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentZ = 1024;

	UPROPERTY(Category = "Top Down Grid", VisibleDefaultsOnly)
	class UBillboardComponent* Billboard = nullptr;

	UPROPERTY(Category = "Top Down Grid", VisibleDefaultsOnly)
	class UBoxComponent* GridVolume = nullptr;

	UPROPERTY(Category = "Config", VisibleAnywhere)
	FTransform GridTransform;
	
	/** 그리드 좌표의 최솟값을 (0, 0)으로 하기 위한 조정값 */
	int GridShift = GridResolution / 2;

	FVector TileExtent;
};
