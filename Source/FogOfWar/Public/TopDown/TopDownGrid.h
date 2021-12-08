// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownGrid.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopDownGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Top Down Grid", BlueprintNativeEvent, BlueprintPure)
	FIntPoint WorldToGrid(const FVector& WorldLocation);
	virtual FIntPoint WorldToGrid_Implementation(const FVector& WorldLocation);

	UFUNCTION(Category = "Top Down Grid", BlueprintNativeEvent, BlueprintPure)
	FVector GridToWorld(const FIntPoint& GridCoords);
	virtual FVector GridToWorld_Implementation(const FIntPoint& GridCoords);

	UFUNCTION(Category = "Top Down Grid", BlueprintPure)
	FVector GetTileExtent() const;

protected:
	UPROPERTY(Category = "Top Down Grid", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 4, ClampMax = 256, UIMin = 4, UIMax = 256))
	int GridResolution = 128;
	int GridShift = 64;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadWrite)
	FTransform GridTransform;

	UPROPERTY(Category = "Top Down Grid", BlueprintReadWrite)
	FVector TileExtent;
};
