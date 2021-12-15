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

		CircleTiles.Reset(CircleTiles.GetSlack());

		// Get circle tiles
		GetBresenhamCircle(AgentCoords, Agent->Sight);

		// 레이캐스트 정확도를 위해 기존 시야보다 작은 원을 하나 더 그립니다.
		GetBresenhamCircle(AgentCoords, Agent->Sight - 1);

		// Classify tiles
		UpdateCachedTiles(AgentCoords);
	}
}

void AFogManager::GetBresenhamCircle(const FIntPoint& Center, int Radius)
{
	if (Radius == 0)
	{
		return;
	}

	int X = 0;
	int Y = Radius;
	int D = 1 - Radius; // Discriminant

	CircleTiles.Add(Center + FIntPoint{  X,  Y });
	CircleTiles.Add(Center + FIntPoint{  X, -Y });
	CircleTiles.Add(Center + FIntPoint{  Y,  X });
	CircleTiles.Add(Center + FIntPoint{ -Y,  X });

	for (X = 1; X < Y; ++X)
	{
		if (D <= 0)
		{
			D += 2 * X + 1;
		}
		else
		{
			D += 2 * X + 1 - 2 * Y;
			--Y;

			/*for (int i = 0; i < X; ++i)
			{
				CircleTiles.Add(Center + FIntPoint{  i,  Y });
				CircleTiles.Add(Center + FIntPoint{ -i,  Y });
				CircleTiles.Add(Center + FIntPoint{  i, -Y });
				CircleTiles.Add(Center + FIntPoint{ -i, -Y });
				CircleTiles.Add(Center + FIntPoint{  Y,  i });
				CircleTiles.Add(Center + FIntPoint{ -Y,  i });
				CircleTiles.Add(Center + FIntPoint{  Y, -i });
				CircleTiles.Add(Center + FIntPoint{ -Y, -i });
			}*/
		}
		CircleTiles.Add(Center + FIntPoint{  X,  Y });
		CircleTiles.Add(Center + FIntPoint{ -X,  Y });
		CircleTiles.Add(Center + FIntPoint{  X, -Y });
		CircleTiles.Add(Center + FIntPoint{ -X, -Y });
		CircleTiles.Add(Center + FIntPoint{  Y,  X });
		CircleTiles.Add(Center + FIntPoint{ -Y,  X });
		CircleTiles.Add(Center + FIntPoint{  Y, -X });
		CircleTiles.Add(Center + FIntPoint{ -Y, -X });
	}

	// 나머지 사각형 부분
	/*for (int i = Center.X - X + 1; i < Center.X + X; ++i)
	{
		for (int j = Center.Y - Y + 1; j < Center.Y + Y; ++j)
		{
			CircleTiles.AddUnique(FIntPoint{ i, j });
		}
	}*/
}

void AFogManager::CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End)
{
	int X = Start.X;
	int Y = Start.Y;

	int DeltaX = FMath::Abs(End.X - Start.X);
	int DeltaY = FMath::Abs(End.Y - Start.Y);

	int XIncreasement = (End.X < Start.X) ? -1 : 1;
	int YIncreasement = (End.Y < Start.Y) ? -1 : 1;

	auto TileData = TopDownGrid->TileData;
	FTile* CenterTile = TileData.Find(Start);
	if (CenterTile == nullptr)
	{
		return;
	}
	FTile* Tile = nullptr;

	if (DeltaY < DeltaX)
	{
		int D = 2 * (DeltaY - DeltaX);

		for (; (Start.X < End.X ? X < End.X : X > End.X); X += XIncreasement)
		{
			if (0 >= D)
			{
				D += 2 * DeltaY;
			}
			else
			{
				D += 2 * DeltaY - 2 * DeltaX;
				Y += YIncreasement;
			}
			// Add tile coords
			Tile = TileData.Find({ X, Y });
			if (Tile == nullptr)
			{
				continue;
			}
			if (CenterTile->Height < Tile->Height)
			{
				break;
			}
			CachedTiles.AddUnique({ X,Y });
		}
	}
	else
	{
		int D = 2 * (DeltaX - DeltaY);

		for (; (Start.Y < End.Y ? Y < End.Y : Y > End.Y); Y += YIncreasement)
		{
			if (0 >= D)
			{
				D += 2 * DeltaX;
			}
			else
			{
				D += 2 * (DeltaX - DeltaY);
				X += XIncreasement;
			}
			// Add tile coords
			Tile = TileData.Find({ X, Y });
			if (Tile == nullptr)
			{
				continue;
			}
			if (CenterTile->Height < Tile->Height)
			{
				break;
			}
			CachedTiles.AddUnique({ X,Y });
		}
	}
}

void AFogManager::UpdateCachedTiles(const FIntPoint& Center)
{
	for (auto& Target : CircleTiles)
	{
		CastBresenhamLine(Center, Target);
	}
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
			DrawDebugBox(GetWorld(), Location, TopDownGrid->GetTileExtent() * 0.5f, FColor::Silver, false, Duration);
		}
	}
}
