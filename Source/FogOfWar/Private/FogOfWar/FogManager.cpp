// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogAgentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rendering/Texture2DResource.h"
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
	GridResolution = TopDownGrid->GetGridResolution();

	ExploredTiles.Reserve(GridResolution * GridResolution);

	FogOfWarTexture.InitFogTexture(GridResolution);

	GetWorldTimerManager().SetTimer(FogUpdateTimer, this, &AFogManager::UpdateFog, 1.0f / static_cast<float>(FogUpdateInterval), true, 0.5f);
}

void AFogManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FogOfWarTexture.ReleaseFogTexture();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AFogManager::UpdateFog()
{
	UpdateFogAgents();
	FogOfWarTexture.UpdateFogTexture();
}

void AFogManager::UpdateFogAgents()
{
	if (TopDownGrid == nullptr)
	{
		return;
	}

	// 탐사한 타일 업데이트
	for (auto& TileCoords : ExploredTiles)
	{
		//FogOfWarTexture.SetBuffer(TileCoords.Y * GridResolution + TileCoords.X, 0x04);
	}

	CachedTiles.Reset(CachedTiles.GetSlack());

	FogOfWarTexture.ResetBuffer();

	for (auto Agent : FogAgents)
	{
		if (Agent == nullptr)
		{
			continue;
		}
		const FIntPoint& AgentCoords = TopDownGrid->WorldToGrid(Agent->GetFogAgentLocation());
		const int AgentSight = TopDownGrid->ConvertToGridUnit(Agent->Sight);
		const FTile* AgentTile = TopDownGrid->TileData.Find(AgentCoords);
		if (AgentTile == nullptr)
		{
			continue;
		}

		CircleTiles.Reset(CircleTiles.GetSlack());

		//// 원에 해당하는 타일을 가져옵니다.
		//GetBresenhamCircle(AgentCoords, Radius);
		//// 레이캐스트 정확도를 위해 기존 시야보다 작은 원에 해당하는 타일을 가져옵니다.
		//GetBresenhamCircle(AgentCoords, Radius - 1);

		//// 중점에서 원까지 직선을 그립니다.
		//for (auto& Target : CircleTiles)
		//{
		//	CastBresenhamLine(AgentCoords, Target);
		//}

		FogOfWarTexture.CalculateFog(AgentCoords, AgentSight, TopDownGrid->IsBlocked);
	}
}

UTexture2D* AFogManager::GetFogTexture() const
{
	return FogOfWarTexture.FogTexture;
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
			Tile = TileData.Find({ X, Y });
			if (Tile == nullptr)
			{
				continue;
			}
			if (CenterTile->Height < Tile->Height)
			{
				CachedTiles.AddUnique({ X, Y });
				ExploredTiles.AddUnique({ X, Y });
				//FogOfWarTexture.SetBuffer(Y * GridResolution + X, 0xFF);
				break;
			}
			CachedTiles.AddUnique({ X, Y });
			ExploredTiles.AddUnique({ X, Y });
			//FogOfWarTexture.SetBuffer(Y * GridResolution + X, 0xFF);
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
			Tile = TileData.Find({ X, Y });
			if (Tile == nullptr)
			{
				continue;
			}
			if (CenterTile->Height < Tile->Height)
			{
				CachedTiles.AddUnique({ X, Y });
				ExploredTiles.AddUnique({ X, Y });
				//FogOfWarTexture.SetBuffer(Y * GridResolution + X, 0xFF);
				break;
			}
			CachedTiles.AddUnique({ X, Y });
			ExploredTiles.AddUnique({ X, Y });
			//FogOfWarTexture.SetBuffer(Y * GridResolution + X, 0xFF);
		}
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
			DrawDebugBox(GetWorld(), Location, TopDownGrid->GetTileExtent() * 0.6f, FColor::Silver, false, Duration);
		}
	}
}
