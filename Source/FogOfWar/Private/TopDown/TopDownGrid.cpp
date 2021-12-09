// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGrid.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATopDownGrid::ATopDownGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	GridVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GridVolume"));
	GridVolume->SetupAttachment(Billboard);
	GridVolume->SetBoxExtent(FVector(512.0f));
}

void ATopDownGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateGridTransform();

	if (bDebugGrid)
	{
		DrawDebugTile();
	}
}

void ATopDownGrid::UpdateGridTransform()
{
	GridResolution = FMath::Clamp<int>(FMath::RoundUpToPowerOfTwo(GridResolution), 4, 256);
	GridShift = GridResolution / 2;

	GridVolume->SetBoxExtent(FVector(GridVolumeScaleXY, GridVolumeScaleXY, GridVolumeScaleZ));

	GridTransform.SetLocation(GridVolume->GetComponentTransform().GetLocation());
	GridTransform.SetRotation(GridVolume->GetComponentTransform().GetRotation());

	float ScaleX = GridVolume->Bounds.BoxExtent.X * 2 / GridResolution;
	float ScaleY = GridVolume->Bounds.BoxExtent.Y * 2 / GridResolution;
	GridTransform.SetScale3D({ScaleX, ScaleY, 1});

	float TileExtentX = GridVolume->Bounds.BoxExtent.X / GridResolution;
	float TileExtentY = GridVolume->Bounds.BoxExtent.Y / GridResolution;
	TileExtent = FVector(TileExtentX, TileExtentY, 0);
}

FIntPoint ATopDownGrid::WorldToGrid(const FVector& WorldLocation)
{
	FVector GridLocation = GridTransform.InverseTransformPosition(WorldLocation);
	
	int X = FMath::Clamp(FMath::FloorToInt(GridLocation.X) + GridShift, 0, GridResolution - 1);
	int Y = FMath::Clamp(FMath::FloorToInt(GridLocation.Y) + GridShift, 0, GridResolution - 1);

	return { X , Y };
}

FVector ATopDownGrid::GridToWorld(const FIntPoint& GridCoords)
{
	FVector GridLocation(GridCoords.X - GridShift, GridCoords.Y - GridShift, 0);
	FVector WorldLocation = GridTransform.TransformPosition(GridLocation);

	WorldLocation += TileExtent;
	WorldLocation.Z = GetActorLocation().Z;

	return WorldLocation;
}

FVector ATopDownGrid::GetTileExtent() const
{
	return TileExtent;
}

void ATopDownGrid::DrawDebugTile()
{
	for (int i = 0; i < GridResolution; ++i)
	{
		for (int j = 0; j < GridResolution; ++j)
		{
			DrawDebugBox(GetWorld(), GridToWorld({ i, j }), TileExtent * 0.95, FColor::Green, true);
		}
	}
}
