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
	/** @return 월드 좌표를 그리드 좌표로 변환합니다. */
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

	/** @return 월드 단위를 그리드 단위로 변환합니다. */
	int ToGridUnit(const int N) const;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadOnly)
	TMap<FIntPoint, FTile> TileData;

	/** Center 타일과 Target 타일의 높이를 비교하는 콜백 함수 */
	TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked;

private:
	void UpdateGridTransform();

	/** @return Returns blocking hit */
	bool CoordsLineTraceToMinusZAxis(const FIntPoint& Coords, ETraceTypeQuery TraceChannel, FHitResult& OutHit);

	/** 타일 맵 생성 */
	void GenerateTileData();

	void DrawDebugGrid();

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugGrid = false;

	UPROPERTY(Category = "Config", EditAnywhere)
	float DebugGridTime = 10.0f;

	/** 그리드 너비에 들어가는 타일 개수 & 안개 텍스처 너비의 텍셀 개수
	* Tile extent = GridVolumeExtentXY / GridResoulution */
	UPROPERTY(Category = "Config", EditAnywhere, meta = (ClampMin = 16, ClampMax = 512, UIMin = 16, UIMax = 512))
	int GridResolution = 256;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentXY = 4096;

	UPROPERTY(Category = "Config", EditAnywhere)
	int GridVolumeExtentZ = 1024;

	UPROPERTY(Category = "Top Down Grid", VisibleDefaultsOnly)
	class UBillboardComponent* Billboard = nullptr;

	UPROPERTY(Category = "Top Down Grid", VisibleDefaultsOnly)
	class UBoxComponent* GridVolume = nullptr;

	/** 그리드 좌표의 트랜스폼. GridVolume을 기준으로 스케일을 조정 */
	FTransform GridTransform;
	
	/** 그리드 좌표의 최솟값을 (0, 0)으로 하기 위한 조정값 */
	int GridShift = GridResolution / 2;

	/** 타일의 반 너비. 50이면 한 변의 길이가 100인 타일 */
	FVector TileExtent;
};
