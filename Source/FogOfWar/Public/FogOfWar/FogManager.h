// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	/** Mid-Point Circle Drawing Algorithm */
	void DrawCircle(const FIntPoint& Center, int Radius);

	void DrawDebugTile(const FLinearColor& Color, float Duration);

	UPROPERTY(Category = "Fog Manager", EditAnywhere, BlueprintReadWrite)
	bool bDebugTile = false;

protected:
	UPROPERTY(Category = "Fog Manager", EditAnywhere, BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<UFogAgentComponent*> FogAgents;

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	TArray<FIntPoint> CircleCoords;
};
