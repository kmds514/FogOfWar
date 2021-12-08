// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogAgentComponent.h"
#include "TopDown/TopDownGrid.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFogManager::AFogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<ATopDownGrid> GridBP;

		FConstructorStatics()
			: GridBP(TEXT("Blueprint'/Game/TopDown/Blueprints/BP_TopDownGrid.BP_TopDownGrid_C'"))
		{}
	};
	FConstructorStatics ConstructorStatics;
	TopDownGrid = ConstructorStatics.GridBP.Class.GetDefaultObject();
}

// Called when the game starts or when spawned
void AFogManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFogAgents();

	if (bDebugTile)
	{
		DrawDebugTile(FLinearColor::Black, DeltaTime * 2.0f);
	}
}

void AFogManager::AddFogAgent(UFogAgentComponent* const FogAgent)
{
	if (FogAgent)
	{
		FogAgents.Add(FogAgent);
		UE_LOG(LogTemp, Log, TEXT("Added fog agent"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid fog agent"));
	}
}

void AFogManager::RemoveFogAgent(UFogAgentComponent* const FogAgent)
{
	if (FogAgent)
	{
		FogAgents.Remove(FogAgent);
		UE_LOG(LogTemp, Log, TEXT("Removed fog agent"));
	}
}

void AFogManager::UpdateFogAgents()
{
	if (TopDownGrid)
	{
		for (auto FogAgent : FogAgents)
		{
			if (FogAgent)
			{
				CircleCoords.Reset(CircleCoords.GetSlack());

				const FIntPoint& Coords = TopDownGrid->WorldToGrid(FogAgent->GetFogAgentLocation());
				DrawCircle(Coords, FogAgent->Sight);
			}
		}
	}
}

void AFogManager::DrawCircle(const FIntPoint& Center, int Radius)
{
	int Discriminant = 1 - Radius;
	int X = 0;
	int Y = Radius;
	CircleCoords.Add(Center + FIntPoint{  X,  Y });
	CircleCoords.Add(Center + FIntPoint{  X, -Y });
	CircleCoords.Add(Center + FIntPoint{  Y,  X });
	CircleCoords.Add(Center + FIntPoint{ -Y,  X });

	++X;

	while (X < Y)
	{
		if (Discriminant < 0)
		{
			Discriminant += 2 * X + 1; // d = 2x + 1 + d
		}
		else
		{
			Discriminant += 2 * X - 2 * Y + 1; // d = 2x - 2y + 1 + d
			--Y;
			for (int i = 0; i < X; ++i)
			{
				CircleCoords.Add(Center + FIntPoint{  i,  Y });
				CircleCoords.Add(Center + FIntPoint{ -i,  Y });
				CircleCoords.Add(Center + FIntPoint{  i, -Y });
				CircleCoords.Add(Center + FIntPoint{ -i, -Y });
				CircleCoords.Add(Center + FIntPoint{  Y,  i });
				CircleCoords.Add(Center + FIntPoint{ -Y,  i });
				CircleCoords.Add(Center + FIntPoint{  Y, -i });
				CircleCoords.Add(Center + FIntPoint{ -Y, -i });
			}
		}
		CircleCoords.Add(Center + FIntPoint{  X,  Y });
		CircleCoords.Add(Center + FIntPoint{ -X,  Y });
		CircleCoords.Add(Center + FIntPoint{  X, -Y });
		CircleCoords.Add(Center + FIntPoint{ -X, -Y });
		CircleCoords.Add(Center + FIntPoint{  Y,  X });
		CircleCoords.Add(Center + FIntPoint{ -Y,  X });
		CircleCoords.Add(Center + FIntPoint{  Y, -X });
		CircleCoords.Add(Center + FIntPoint{ -Y, -X });
	}

	for (int i = Center.X - X + 1; i < Center.X + X; ++i)
	{
		for (int j = Center.Y - Y + 1; j < Center.Y + Y; ++j)
		{
			CircleCoords.Add(Center + FIntPoint{ i, j });

		}
	}
}

void AFogManager::DrawDebugTile(const FLinearColor& Color, float Duration)
{
	for (const auto& Coords : CircleCoords)
	{
		FVector Location = TopDownGrid->GridToWorld(Coords);
		UKismetSystemLibrary::DrawDebugPoint(GetWorld(), Location, TopDownGrid->GetTileExtent().X * 0.95f, Color, Duration);
	}
}
