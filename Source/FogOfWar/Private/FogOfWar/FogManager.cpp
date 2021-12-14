// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogAgentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownGrid::StaticClass(), OutActors);
	if (OutActors.Num() == 1)
	{
		TopDownGrid = Cast<ATopDownGrid>(OutActors[0]);

		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Log, TEXT("%s: Successfully get TopDownGrid."), *Name);
	}
	else
	{
		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Error, TEXT("%s: TopDownGrid must exist only one instance in world. Current instance is %d "), *Name, OutActors.Num());
	}
}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFogAgents();

	if (bDebugTile)
	{
		DrawDebugTile(DeltaTime * 2.0f);
	}
}

void AFogManager::AddFogAgent(UFogAgentComponent* const FogAgent)
{
	if (FogAgent)
	{
		FogAgents.Add(FogAgent);

		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Log, TEXT("%s: Added fog agent"), *Name);

	}
	else
	{
		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Warning, TEXT("%s: Invalid fog agent"), *Name);
	}
}

void AFogManager::RemoveFogAgent(UFogAgentComponent* const FogAgent)
{
	if (FogAgent)
	{
		FogAgents.Remove(FogAgent);

		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Log, TEXT("%s: Removed fog agent"), *Name);
	}
}

void AFogManager::UpdateFogAgents()
{
	if (TopDownGrid == nullptr)
	{
		return;
	}

	CachedTiles.Reset(CachedTiles.GetSlack());

	for (auto Agent : FogAgents)
	{
		if (Agent == nullptr)
		{
			continue;
		}
		const FIntPoint& AgentCoords = TopDownGrid->WorldToGrid(Agent->GetFogAgentLocation());
		const FTile* AgentTile = TopDownGrid->TileData.Find(AgentCoords);
		if (AgentTile == nullptr)
		{
			continue;
		}
		GetCircleQuadrantZero(AgentCoords, Agent->Sight);
	}
}

void AFogManager::GetBresenhamCircle(const FIntPoint& Center, int Radius, int AgentHeight)
{
	int X = 0;
	int Y = Radius;
	int D = 1 - Radius; // Discriminant

	CachedTiles.AddUnique(Center + FIntPoint{  X,  Y });
	CachedTiles.AddUnique(Center + FIntPoint{  X, -Y });
	CachedTiles.AddUnique(Center + FIntPoint{  Y,  X });
	CachedTiles.AddUnique(Center + FIntPoint{ -Y,  X });

	for (X = 1; X < Y; ++X)
	{
		if (D < 0)
		{
			D += 2 * X + 1;
		}
		else
		{
			D += 2 * X + 1 - 2 * Y;
			--Y;

			for (int i = 0; i < X; ++i)
			{
				CachedTiles.AddUnique(Center + FIntPoint{  i,  Y });
				CachedTiles.AddUnique(Center + FIntPoint{ -i,  Y });
				CachedTiles.AddUnique(Center + FIntPoint{  i, -Y });
				CachedTiles.AddUnique(Center + FIntPoint{ -i, -Y });
				CachedTiles.AddUnique(Center + FIntPoint{  Y,  i });
				CachedTiles.AddUnique(Center + FIntPoint{ -Y,  i });
				CachedTiles.AddUnique(Center + FIntPoint{  Y, -i });
				CachedTiles.AddUnique(Center + FIntPoint{ -Y, -i });
			}
		}
		CachedTiles.AddUnique(Center + FIntPoint{  X,  Y });
		CachedTiles.AddUnique(Center + FIntPoint{ -X,  Y });
		CachedTiles.AddUnique(Center + FIntPoint{  X, -Y });
		CachedTiles.AddUnique(Center + FIntPoint{ -X, -Y });
		CachedTiles.AddUnique(Center + FIntPoint{  Y,  X });
		CachedTiles.AddUnique(Center + FIntPoint{ -Y,  X });
		CachedTiles.AddUnique(Center + FIntPoint{  Y, -X });
		CachedTiles.AddUnique(Center + FIntPoint{ -Y, -X });
	}

	for (int i = Center.X - X + 1; i < Center.X + X; ++i)
	{
		for (int j = Center.Y - Y + 1; j < Center.Y + Y; ++j)
		{
			CachedTiles.AddUnique(FIntPoint{ i, j });
		}
	}
}

void AFogManager::GetBresenhamLine(const FIntPoint& Start, const FIntPoint& End)
{
	int X = Start.X;
	int Y = Start.Y;

	int DeltaX = FMath::Abs(End.X - Start.X);
	int DeltaY = FMath::Abs(End.Y - Start.Y);

	int XIncreasement = (End.X < Start.X) ? -1 : 1;
	int YIncreasement = (End.Y < Start.Y) ? -1 : 1;

	if (DeltaY < DeltaX)
	{
		int Discriminant = 2 * (DeltaY - DeltaX);

		for (; (Start.X < End.X ? X < End.X : X > End.X); X += XIncreasement)
		{
			if (0 >= Discriminant)
			{
				Discriminant += 2 * DeltaY;
			}
			else
			{
				Discriminant += 2 * (DeltaY - DeltaX);
				Y += YIncreasement;
			}
			// Add tile coords
		}
	}
	else
	{
		int Discriminant = 2 * (DeltaX - DeltaY);

		for (; (Start.Y < End.Y ? Y < End.Y : Y > End.Y); Y += YIncreasement)
		{
			if (0 >= Discriminant)
			{
				Discriminant += 2 * DeltaX;
			}
			else
			{
				Discriminant += 2 * (DeltaX - DeltaY);
				X += XIncreasement;
			}
			// Add tile coords
		}
	}
}

void AFogManager::GetCircleArea(const FIntPoint& Center, int Radius)
{
	// 1. Center를 기준으로 Extent가 Radius인 사각형 타일을 구한다.
	// 2. for X in 사각형 :
	// 2-1. 왼쪽 타일부터 Center와의 거리 D를 계산한다.
	// 2-2. D <= Radius면 멈추고 멈춘 위치 L을 저장한다.
	// 2-3. 오른쪽 타일부터 Center와의 거리 D를 계산한다.
	// 2-4. D <= Radius면 멈추고 멈춘 위치 R을 저장한다.
	// 2-5. (X, L)부터 (X, R)까지의 타일이 원에 해당하는 타일이다.

	// ^ X-Axis
	// |
	// |
	// |- - - > Y-Axis
	const int Left = Center.Y - Radius + 1;		
	const int Right = Center.Y + Radius - 1;	
	const int Top = Center.X + Radius - 1;		
	const int Bottom = Center.X - Radius + 1;	

	const float RadiuSquared = Radius * Radius /* 원이 부드럽게 그려지도록 값을 살짝 줄인다. */ * 0.95f;
	
	for (int X = Bottom; X <= Top; ++X)
	{
		int LeftY = Left;
		
		float Dist = (X - Center.X) * (X - Center.X) + (LeftY - Center.Y) * (LeftY - Center.Y);
		while (Dist > RadiuSquared)
		{
			++LeftY;
			Dist = (X - Center.X) * (X - Center.X) + (LeftY - Center.Y) * (LeftY - Center.Y);
		}
		
		int RightY = Right;
		Dist = FVector2D::DistSquared(Center, FIntPoint{ X, RightY });
		while (Dist > RadiuSquared)
		{
			--RightY;
			Dist = FVector2D::DistSquared(Center, FIntPoint{ X, RightY });
		}

		for (int i = LeftY; i <= RightY; ++i)
		{
			// Add tile coords
			CachedTiles.Add({ X, i });
		}
	}
}

void AFogManager::GetCircleQuadrantZero(const FIntPoint& Center, const int Radius)
{
	TQueue<FColumnPortion> Columns;

	FIntPoint TopVector = { Center.X + Radius, Center.Y };
	FIntPoint BottomVector = { Center.X, Center.Y + Radius };
	Columns.Enqueue({ Center.Y, TopVector, BottomVector });

	while (Columns.IsEmpty() == false)
	{
		FColumnPortion ColumnPortion = {};
		Columns.Dequeue(ColumnPortion);
		FIntPoint Prev = { TNumericLimits<int32>::Max(), TNumericLimits<int32>::Max() };

		for (int X = ColumnPortion.TopVector.X; X >= ColumnPortion.BottomVector.X; --X)
		{
			FIntPoint Curr = { X, ColumnPortion.Column };

			if (IsCircle(Center, Curr, Radius) == false)
			{
				continue;
			}

			if (IsObstacle(Center, Curr) || IsFloor(Center, Curr))
			{
				CachedTiles.Add(Curr);
			}
			if (IsObstacle(Center, Prev) && IsFloor(Center, Curr))
			{
				// TopVector 기울기 감소
			}
			if (IsFloor(Center, Prev) && IsObstacle(Center, Curr))
			{
				// Get next column
				FColumnPortion NextColumnPortion = ColumnPortion.Next();
				
				// next column BottomVector 기울기 증가

				// Enqueue next column
				Columns.Enqueue(NextColumnPortion);
			}
			Prev = Curr;
		}
		if (IsFloor(Center, Prev))
		{
			Columns.Enqueue(ColumnPortion.Next());
		}
	}
}

bool AFogManager::IsObstacle(const FIntPoint& Center, const FIntPoint& Tile) const
{
	const FTile* CenterData = TopDownGrid->TileData.Find(Center);
	const FTile* TileData = TopDownGrid->TileData.Find(Tile);

	if (CenterData == nullptr || TileData == nullptr)
	{
		return false;
	}

	if (TileData->Height > CenterData->Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AFogManager::IsFloor(const FIntPoint& Center, const FIntPoint& Tile) const
{
	const FTile* CenterData = TopDownGrid->TileData.Find(Center);
	const FTile* TileData = TopDownGrid->TileData.Find(Tile);

	if (CenterData == nullptr || TileData == nullptr)
	{
		return false;
	}

	if (TileData->Height <= CenterData->Height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AFogManager::IsSymmetric(const int Column, const FIntPoint& Tile, const FIntPoint& TopVector, const FIntPoint& BottomVector) const
{
	return false;
}

bool AFogManager::IsCircle(const FIntPoint& Center, const FIntPoint& Tile, const int Radius) const
{
	int DistX = Tile.X - Center.X;
	int DistY = Tile.Y - Center.Y;

	int SqrR = FMath::Square<int>(Radius)/* * 0.95 */;
	int SqrDx = FMath::Square<int>(DistX);
	int SqrDy = FMath::Square<int>(DistY);

	return SqrDx + SqrDy < SqrR;
}

void AFogManager::DrawDebugTile(float Duration)
{
	if (TopDownGrid == nullptr)
	{
		return;
	}

	for (const auto& Coords : CachedTiles)
	{
		FTile* Tile = TopDownGrid->TileData.Find(Coords);
		if (Tile)
		{
			FVector Location = Tile->WorldLocation;
			Location.Z += 1.0f;
			DrawDebugBox(GetWorld(), Location, TopDownGrid->GetTileExtent() * 0.95f, FColor::Green, false, Duration);
		}
	}
}
