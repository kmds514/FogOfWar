// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGrid.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#define ECC_Lowest ECC_GameTraceChannel2
#define ECC_Low ECC_GameTraceChannel3
#define ECC_Medium ECC_GameTraceChannel4
#define ECC_High ECC_GameTraceChannel5
#define ECC_Highest ECC_GameTraceChannel6

ATopDownGrid::ATopDownGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	GridVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("GridVolume"));
	GridVolume->SetupAttachment(Billboard);
	GridVolume->SetBoxExtent(FVector(GridVolumeExtentXY, GridVolumeExtentXY, GridVolumeExtentZ));

	// TMap 크기 예약
	TileData.Reserve(GridResolution * GridResolution);

	// 콜백 함수 정의
	IsBlocked = [&](const FIntPoint& Center, const FIntPoint& Target)
	{
		auto C = TileData.Find(Center);
		auto T = TileData.Find(Target);

		if (C && T)
		{
			return C->Height < T->Height;
		}
		return false;
	};
}

void ATopDownGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateGridTransform();
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

int ATopDownGrid::ToGridUnit(const int N) const
{
	return N / GridTransform.GetScale3D().X;
}

void ATopDownGrid::UpdateGridTransform()
{
	GridResolution = FMath::Clamp<int>(FMath::RoundUpToPowerOfTwo(GridResolution), 16, 512);
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

bool ATopDownGrid::CoordsLineTraceToMinusZAxis(const FIntPoint& Coords, ETraceTypeQuery TraceChannel, FHitResult& OutHit)
{
	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;
	FVector GridLocation = FVector::ZeroVector;

	GridLocation.X = Coords.X - GridShift;
	GridLocation.Y = Coords.Y - GridShift;

	// Grid location to world location
	Start = GridTransform.TransformPosition(GridLocation) + TileExtent;

	// Grid volume's top
	Start.Z = GridVolume->GetComponentLocation().Z + GridVolumeExtentZ;

	// Grid volume's bottom
	End.X = Start.X;
	End.Y = Start.Y; 
	End.Z = GridVolume->GetComponentLocation().Z - GridVolumeExtentZ;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, TArray<AActor*>(), EDrawDebugTrace::None, OutHit, true);

	return OutHit.bBlockingHit;
}

void ATopDownGrid::GenerateTileData()
{
	// Empty map but preserve allocation and capacity
	TileData.Reset();

	// 라인 트레이스 충돌 지점으로 높이 값을 분류하며 그리드 맵 생성
	for (int i = 0; i < GridResolution; i++)
	{
		for (int j = 0; j < GridResolution; j++)
		{
			FHitResult OutHit = {};
			if (CoordsLineTraceToMinusZAxis({ i, j }, UEngineTypes::ConvertToTraceType(ECC_Lowest), OutHit))
			{
				FTile Tile = {};
				Tile.WorldLocation = OutHit.ImpactPoint;
				Tile.Height = ETileHeight::Lowest;

				TileData.Add(FIntPoint{ i, j }, Tile);
			}
			if (CoordsLineTraceToMinusZAxis({ i, j }, UEngineTypes::ConvertToTraceType(ECC_Low), OutHit))
			{
				FTile Tile = {};
				Tile.WorldLocation = OutHit.ImpactPoint;
				Tile.Height = ETileHeight::Low;

				TileData.Add(FIntPoint{ i, j }, Tile);
			}
			if (CoordsLineTraceToMinusZAxis({ i, j }, UEngineTypes::ConvertToTraceType(ECC_Medium), OutHit))
			{
				FTile Tile = {};
				Tile.WorldLocation = OutHit.ImpactPoint;
				Tile.Height = ETileHeight::Medium;

				TileData.Add(FIntPoint{ i, j }, Tile);
			}
			if (CoordsLineTraceToMinusZAxis({ i, j }, UEngineTypes::ConvertToTraceType(ECC_High), OutHit))
			{
				FTile Tile = {};
				Tile.WorldLocation = OutHit.ImpactPoint;
				Tile.Height = ETileHeight::High;

				TileData.Add(FIntPoint{ i, j }, Tile);
			}
			if (CoordsLineTraceToMinusZAxis({ i, j }, UEngineTypes::ConvertToTraceType(ECC_Highest), OutHit))
			{
				FTile Tile = {};
				Tile.WorldLocation = OutHit.ImpactPoint;
				Tile.Height = ETileHeight::Highest;

				TileData.Add(FIntPoint{ i, j }, Tile);
			}
		}
	}
}

FVector ATopDownGrid::GetTileExtent() const
{
	return TileExtent;
}

int ATopDownGrid::GetGridResolution() const
{
	return GridResolution;
}

int ATopDownGrid::GetGridVolumeExtentXY() const
{
	return GridVolumeExtentXY;
}

int ATopDownGrid::GetGridVolumeExtentZ() const
{
	return GridVolumeExtentZ;
}

void ATopDownGrid::DrawDebugGrid()
{
	if (bDebugGrid == false)
	{
		return;
	}

	for (auto Tile : TileData)
	{
		FVector Location = Tile.Value.WorldLocation;
		Location.Z += 1.0f;

		DrawDebugBox(GetWorld(), Location, TileExtent * 0.8f, FColor::Green, false, DebugGridTime);
	}
}
