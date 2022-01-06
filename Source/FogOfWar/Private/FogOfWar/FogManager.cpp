// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogTexture.h"

#include "TopDown/TopDownPlayerController.h"
#include "TopDown/TopDownUnit.h"
#include "TopDown/TopDownGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Rendering/Texture2DResource.h"
#include "DrawDebugHelpers.h"

AFogManager::AFogManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFogManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() == nullptr)
	{
		return;
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownGrid::StaticClass(), OutActors);
	if (OutActors.Num() == 1)
	{
		TopDownGrid = Cast<ATopDownGrid>(OutActors[0]);
	}
	else
	{
		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Error, TEXT("%s: TopDownGrid must exist only one instance in world. Current instance is %d "), *Name, OutActors.Num());
		return;
	}
	GridResolution = TopDownGrid->GetGridResolution();

	FogTexture = new FFogTexture();
	FogTexture->InitFogTexture(GridResolution);

	GetWorldTimerManager().SetTimer(FogUpdateTimer, this, &AFogManager::UpdateFog, 1.0f / static_cast<float>(FogUpdateInterval), true, 0.5f);
}

void AFogManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FogTexture->ReleaseFogTexture();
	delete FogTexture;

	Super::EndPlay(EndPlayReason);
}

void AFogManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFogManager::UpdateFog()
{
	if (TopDownGrid == nullptr)
	{
		return;
	}

	// 안개 텍스처 업데이트
	UpdateFogTexture();

	// 안개에 따른 유닛 가시성 업데이트
	UpdateUnitVisibility();
}

void AFogManager::UpdateFogTexture()
{
	FogTexture->UpdateExploredFog();

	// PC가 소유한 유닛들만 시야 계산
	auto TopDownPC = Cast<ATopDownPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Invalid TopDownPC"));
		return;
	}
	for (auto Unit : TopDownPC->OwningUnits)
	{
		if (Unit == nullptr)
		{
			continue;
		}

		const FIntPoint& UnitCoords = TopDownGrid->WorldToGrid(Unit->GetActorLocation());
		const int UnitSight = TopDownGrid->ToGridUnit(Unit->Sight);
		FogTexture->UpdateFogBuffer(UnitCoords, UnitSight, TopDownGrid->IsBlocked);
	}

	FogTexture->UpdateFogTexture();
}

void AFogManager::UpdateUnitVisibility()
{
	// Get TopDownGS
	auto TopDownGS = Cast<ATopDownGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (TopDownGS == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Invalid TopDownGS"));
		return;
	}

	// Get TopDownPC
	auto TopDownPC = Cast<ATopDownPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Invalid TopDownPC"));
		return;
	}

	// 모든 TopDownUnit 순회
	for (auto Unit : TopDownGS->AllUnits)
	{
		if (Unit == nullptr)
		{
			continue;
		}

		// PC와 같은 팀 유닛이면 가시성 검사 안함
		if (Unit->GetGenericTeamId() == TopDownPC->TeamId)
		{
			continue;
		}

		// 다른 팀 유닛이면 그 유닛의 그리드 좌표를 확인하여 유닛의 가시성 결정
		const FIntPoint& UnitCoords = TopDownGrid->WorldToGrid(Unit->GetActorLocation());
		if (FogTexture->IsRevealed(UnitCoords))
		{
			//TopDownGS->Client_SetUnitHiddenInGame(Unit, false);
		}
		else
		{
			//TopDownGS->Client_SetUnitHiddenInGame(Unit, false);
		}
	}
}

UTexture2D* AFogManager::GetFogTexture() const
{
	return FogTexture->FogTexture;
}
