// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDown/TopDownGrid.h"

#include <unordered_map>
#include <array>

#include "FogManager.generated.h"

class UFogAgentComponent;

UCLASS()
class FOGOFWAR_API AFogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AFogManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	void AddFogAgent(UFogAgentComponent* const FogAgent);
	void RemoveFogAgent(UFogAgentComponent* const FogAgent);
	void UpdateFogAgents();

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly, Transient)
	UTexture2D* FogTexture = nullptr;

protected:
	/** https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
	void GetBresenhamCircle(const FIntPoint& Center, int Radius);

	/** https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
	void CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End);

	void UpdateFog();

	void InitializeFogUpscaling();

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<UFogAgentComponent*> FogAgents;

	UPROPERTY()
	TArray<FIntPoint> CircleTiles;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<FIntPoint> CachedTiles;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<FIntPoint> ExploredTiles;

	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugTile = false;

	/** Number of fog updates per second */
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax ="60", UIMin = "1", UIMax = "60"))
	uint8 FogUpdateInterval = 10;

	void DrawDebugTile(float Duration);

	void InitializeFogTexture();
	void UpdateFogTexture(uint8* const Buffer, const uint32 FogBufferSize, UTexture2D* const FogTexture, FUpdateTextureRegion2D* const FogUpdateRegion);
	void ReleaseFogTexture();

	uint32 GridResolution = 0;

	FTimerHandle FogUpdateTimer;
	
	uint8* FogBuffer = nullptr;
	uint32 FogBufferSize = 0;
	FUpdateTextureRegion2D FogUpdateRegion;

	struct FFogTextureContext
	{
		FTexture2DResource* TextureResource;
		uint32 MipIndex;
		FUpdateTextureRegion2D* UpdateRegion;
		uint32 SourcePitch; 
		uint8* SourceData;
	};

	using FogTexel2X2 = std::array<uint8, 4>;
	using FogTexel4X4 = std::array<std::array<uint8, 4>, 4>;

	struct FCustomHash
	{
		std::size_t operator()(const FogTexel2X2& Texel) const
		{
			return std::hash<int>{}(Texel[0] * 1000 + Texel[1] * 100 + Texel[2] * 10 + Texel[3]);
		}
	};

	// https://technology.riotgames.com/sites/default/files/fow_diagram.png
	std::unordered_map<FogTexel2X2, FogTexel4X4, FCustomHash> FogUpscaling;
};
