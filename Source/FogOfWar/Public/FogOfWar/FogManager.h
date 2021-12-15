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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddFogAgent(UFogAgentComponent* const FogAgent);
	void RemoveFogAgent(UFogAgentComponent* const FogAgent);
	void UpdateFogAgents();

	void GetBresenhamCircle(const FIntPoint& Center, int Radius);

	void CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End);

protected:
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite)
	bool bDebugTile = false;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<UFogAgentComponent*> FogAgents;

	UPROPERTY()
	TArray<FIntPoint> CircleTiles;

	UPROPERTY()
	TArray<FIntPoint> CachedTiles;

private:
	void UpdateCachedTiles(const FIntPoint& Center);

	void DrawDebugTile(float Duration);
};
