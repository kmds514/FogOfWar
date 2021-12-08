// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGrid.h"

// Sets default values
ATopDownGrid::ATopDownGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATopDownGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopDownGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FIntPoint ATopDownGrid::WorldToGrid_Implementation(const FVector& WorldLocation)
{
	return FIntPoint();
}

FVector ATopDownGrid::GridToWorld_Implementation(const FIntPoint& GridCoords)
{
	return FVector();
}

FVector ATopDownGrid::GetTileExtent() const
{
	return TileExtent;
}
