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

	void GetBresenhamCircle(const FIntPoint& Center, int Radius, int AgentHeight);

	void GetBresenhamLine(const FIntPoint& Start, const FIntPoint& End);

	void GetCircleArea(const FIntPoint& Center, int Radius);

	void GetCircleQuadrantZero(const FIntPoint& Center, const int Radius);

	/** @return Tile이 원 안에 있는지 확인합니다. */
	bool IsCircle(const FIntPoint& Center, const FIntPoint& Tile, const int Radius) const;

	/** @return Tile이 장애물 타일인지 확인합니다. */
	bool IsObstacle(const FIntPoint& Center, const FIntPoint& Tile) const;
	
	/** @return Tile이 바닥 타일인지 확인합니다. */
	bool IsFloor(const FIntPoint& Center, const FIntPoint& Tile) const;

	/** @return Tile이 Center에서 대칭으로 보일 수 있는지 확인합니다. */
	bool IsSymmetric(const int Column, const FIntPoint& Tile, const FIntPoint& TopVector, const FIntPoint& BottomVector) const;

protected:
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite)
	bool bDebugTile = false;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<UFogAgentComponent*> FogAgents;

	UPROPERTY()
	TArray<FIntPoint> CachedTiles;

private:
	void DrawDebugTile(float Duration);
};

USTRUCT(Atomic)
struct FColumnPortion
{
	GENERATED_BODY()

	FColumnPortion() = default;
	FColumnPortion(int Column, const FIntPoint& TopVector, const FIntPoint& BottomVector)
		: Column(Column), TopVector(TopVector), BottomVector(BottomVector)
	{}

	FColumnPortion Next()
	{
		return FColumnPortion{ Column + 1, TopVector, BottomVector };
	}

	int Column;
	FIntPoint TopVector;
	FIntPoint BottomVector;
};
