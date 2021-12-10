// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGrid.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#define ECC_Terrain ECC_GameTraceChannel1
#define ECC_Obstacle ECC_GameTraceChannel2

// Sets default values
ATopDownGrid::ATopDownGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	GridVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GridVolume"));
	GridVolume->SetupAttachment(Billboard);
	GridVolume->SetBoxExtent(FVector(GridVolumeExtentXY, GridVolumeExtentXY, GridVolumeExtentZ));
}

void ATopDownGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateGridTransform();

	GenerateTileData();

	if (bDebugGrid)
	{
		DrawDebugGrid();
	}
}

void ATopDownGrid::BeginPlay()
{
	Super::BeginPlay();

	UpdateGridTransform();
	GenerateTileData();

	if (bDebugGrid)
	{
		DrawDebugGrid();
	}
}

void ATopDownGrid::UpdateGridTransform()
{
	GridResolution = FMath::Clamp<int>(FMath::RoundUpToPowerOfTwo(GridResolution), 4, 256);
	GridShift = GridResolution / 2;

	GridVolume->SetBoxExtent(FVector(GridVolumeExtentXY, GridVolumeExtentXY, GridVolumeExtentZ));

	GridTransform.SetLocation(GridVolume->GetComponentTransform().GetLocation());
	GridTransform.SetRotation(GridVolume->GetComponentTransform().GetRotation());

	float GridUnit = GridVolumeExtentXY * 2 / GridResolution;
	GridTransform.SetScale3D({ GridUnit, GridUnit, 1});

	float TileExtentXY = GridVolumeExtentXY / GridResolution;
	TileExtent = FVector(TileExtentXY, TileExtentXY, 0);
}

FIntPoint ATopDownGrid::WorldToGrid(const FVector& WorldLocation) const
{
	FVector GridLocation = GridTransform.InverseTransformPosition(WorldLocation);
	
	int X = FMath::Clamp(FMath::FloorToInt(GridLocation.X) + GridShift, 0, GridResolution - 1);
	int Y = FMath::Clamp(FMath::FloorToInt(GridLocation.Y) + GridShift, 0, GridResolution - 1);

	return { X, Y };
}

FVector ATopDownGrid::GridToWorld(const FIntPoint& GridCoords) const
{
	const FTile* const Tile = TileData.Find(GridCoords);
	if (Tile)
	{
		return Tile->WorldLocation;
	}

	FString Name;
	GetName(Name);
	UE_LOG(LogTemp, Warning, TEXT("%s: Can't find tile"), *Name);
	return FVector::ZeroVector;
}

FVector ATopDownGrid::CoordsLineTraceToMinusZAxis(const FIntPoint& Coords)
{
	FVector GridLocation(Coords.X - GridShift, Coords.Y - GridShift, 0);

	// Grid to world
	FVector Start = GridTransform.TransformPosition(GridLocation);
	Start += TileExtent;

	// Set z to grid volume's top
	Start.Z = GridVolume->GetComponentLocation().Z + GridVolumeExtentZ;

	// Grid volume's bottom
	FVector End = { Start.X, Start.Y, GridVolume->GetComponentLocation().Z - GridVolumeExtentZ };

	auto TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Terrain);
	auto DrawDebugType = bDebugLineTrace ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None;
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, TArray<AActor*>(), DrawDebugType, OutHit, true);

	return OutHit.ImpactPoint;
}

void ATopDownGrid::GenerateTileData()
{
	TileData.Reset();

	for (int i = 0; i < GridResolution; i++)
	{
		for (int j = 0; j < GridResolution; j++)
		{
			FVector WorldLocation = CoordsLineTraceToMinusZAxis({ i, j });

			int Height = FMath::FloorToInt(WorldLocation.Z / HeightDivisor);

			TileData.Add(FIntPoint{ i, j }, FTile{ WorldLocation, Height, false, true });
		}
	}
}

FVector ATopDownGrid::GetTileExtent() const
{
	return TileExtent;
}

void ATopDownGrid::DrawDebugGrid()
{
	if (bDebugGrid == false)
	{
		return;
	}

	for (int i = 0; i < GridResolution; i++)
	{
		for (int j = 0; j < GridResolution; j++)
		{
			FVector Location = GridToWorld({ i, j });
			Location.Z += 1.0f;

			DrawDebugBox(GetWorld(), Location, TileExtent * 0.95f, FColor::Green, true);
			//DrawDebugPoint(GetWorld(), WorldLocation, TileExtent.X * 0.5f, FColor::Green, true);
		}
	}
}