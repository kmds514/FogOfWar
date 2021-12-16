// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDown/TopDownGrid.h"
#include "FogManager.generated.h"

class UFogAgentComponent;

UCLASS()
class FOGOFWAR_API AFogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddFogAgent(UFogAgentComponent* const FogAgent);
	void RemoveFogAgent(UFogAgentComponent* const FogAgent);
	void UpdateFogAgents();

	/** https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
	void GetBresenhamCircle(const FIntPoint& Center, int Radius);

	/** https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
	void CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End);

protected:
	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<UFogAgentComponent*> FogAgents;

	UPROPERTY()
	TArray<FIntPoint> CircleTiles;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<FIntPoint> CachedTiles;

private:
	UPROPERTY(Category = "Config", EditAnywhere)
	bool bDebugTile = false;

	void UpdateCachedTiles(const FIntPoint& Center);

	void DrawDebugTile(float Duration);

	FTimerHandle FogTimer;

	uint8* FogBuffer = nullptr;
	int FogBufferSize = 0;
	int BufferSize = 0;
};
