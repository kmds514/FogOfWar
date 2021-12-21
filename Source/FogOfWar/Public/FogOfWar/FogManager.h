// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDown/TopDownGrid.h"
#include "FogOfWar/FogTexture.h"
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

	UFUNCTION(Category = "Fog Manager", BlueprintPure)
	UTexture2D* GetFogTexture() const;

protected:
	/** https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
	void GetBresenhamCircle(const FIntPoint& Center, int Radius);

	/** https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
	void CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End);

	void UpdateFog();

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

	uint32 GridResolution = 0;

	FTimerHandle FogUpdateTimer;

	FFogTexture FogOfWarTexture;
};
