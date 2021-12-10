// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogAgentComponent.h"
#include "TopDown/TopDownGrid.h"
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
		DrawDebugTile(FColor::Black, DeltaTime * 2.0f);
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
	if (TopDownGrid)
	{
		for (auto Agent : FogAgents)
		{
			if (Agent)
			{
				CachedCoords.Reset(CachedCoords.GetSlack());

				const FIntPoint& Coords = TopDownGrid->WorldToGrid(Agent->GetFogAgentLocation());
				DrawBresenhamCircle(Coords, Agent->Sight);
			}
		}
	}
}

void AFogManager::DrawBresenhamCircle(const FIntPoint& Center, int Radius)
{
	int Discriminant = 1 - Radius;
	int X = 0;
	int Y = Radius;

	DrawBresenhamLine(Center, Center + FIntPoint{  X,  Y }); // {  0,  R  }
	DrawBresenhamLine(Center, Center + FIntPoint{  X, -Y }); // {  0, -R  }
	DrawBresenhamLine(Center, Center + FIntPoint{  Y,  X }); // {  R,  0  }
	DrawBresenhamLine(Center, Center + FIntPoint{ -Y,  X }); // { -R,  0  }

	++X;

	while (X < Y)
	{
		if (Discriminant < 0)
		{
			Discriminant += 2 * X + 1; // d = d + 2x + 1
		}
		else
		{
			Discriminant += 2 * X - 2 * Y + 1; // d = d + 2x - 2y + 1

			--Y;

			/*for (int i = 0; i < X; ++i)
			{
				CircleCoords.Add(Center + FIntPoint{  i,  Y });
				CircleCoords.Add(Center + FIntPoint{ -i,  Y });
				CircleCoords.Add(Center + FIntPoint{  i, -Y });
				CircleCoords.Add(Center + FIntPoint{ -i, -Y });
				CircleCoords.Add(Center + FIntPoint{  Y,  i });
				CircleCoords.Add(Center + FIntPoint{ -Y,  i });
				CircleCoords.Add(Center + FIntPoint{  Y, -i });
				CircleCoords.Add(Center + FIntPoint{ -Y, -i });
			}*/
		}
		DrawBresenhamLine(Center, Center + FIntPoint{  X,  Y });
		DrawBresenhamLine(Center, Center + FIntPoint{ -X,  Y });
		DrawBresenhamLine(Center, Center + FIntPoint{  X, -Y });
		DrawBresenhamLine(Center, Center + FIntPoint{ -X, -Y });
		DrawBresenhamLine(Center, Center + FIntPoint{  Y,  X });
		DrawBresenhamLine(Center, Center + FIntPoint{ -Y,  X });
		DrawBresenhamLine(Center, Center + FIntPoint{  Y, -X });
		DrawBresenhamLine(Center, Center + FIntPoint{ -Y, -X });

		++X;
	}

	/*for (int i = Center.X - X + 1; i < Center.X + X; ++i)
	{
		for (int j = Center.Y - Y + 1; j < Center.Y + Y; ++j)
		{
			CircleCoords.Add(Center + FIntPoint{ i, j });
		}
	}*/
}

void AFogManager::DrawBresenhamLine(const FIntPoint& Start, const FIntPoint& End)
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
			CachedCoords.AddUnique({ X, Y });
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
			CachedCoords.AddUnique({ X, Y });
		}
	}
}

void AFogManager::DrawDebugTile(const FColor& Color, float Duration)
{
	for (const auto& Coords : CachedCoords)
	{
		if (TopDownGrid)
		{
			FVector Location = TopDownGrid->GridToWorld(Coords);
			DrawDebugPoint(GetWorld(), Location, TopDownGrid->GetTileExtent().X * 0.95f, Color, false, Duration);
		}
	}
}
