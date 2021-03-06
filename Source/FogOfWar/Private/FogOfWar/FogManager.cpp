// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogManager.h"
#include "FogOfWar/FogTexture.h"

#include "TopDown/TopDownPlayerController.h"
#include "TopDown/TopDownUnit.h"

#include "Kismet/GameplayStatics.h"

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

	// Get TopDownGrid
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

	// Get grid resoultion
	GridResolution = TopDownGrid->GetGridResolution();

	// Initialize FogTexture class
	FogTexture = new FFogTexture();
	FogTexture->InitFogTexture(GridResolution);

	// Set fog update timer
	GetWorldTimerManager().SetTimer(FogUpdateTimer, this, &AFogManager::UpdateFog, 1.0f / static_cast<float>(FogUpdateInterval), true, 0.5f);
}

void AFogManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Release FogTexture class
	FogTexture->ReleaseFogTexture();
	delete FogTexture;

	Super::EndPlay(EndPlayReason);
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
	Client_UpdateUnitVisibility();
}

void AFogManager::UpdateFogTexture()
{
	// 한 번이라도 탐사한 타일이 있는지 업데이트
	FogTexture->UpdateExploredFog();
	
	// Get TopDownPC
	auto TopDownPC = Cast<ATopDownPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(": Invalid TopDownPC"));
		return;
	}

	// TopDownPC가 소유한 유닛들만 시야 계산
	for (auto Unit : TopDownPC->OwningUnits)
	{
		if (Unit == nullptr)
		{
			continue;
		}
		// 유닛의 위치를 그리드 좌표로 변환
		const FIntPoint& UnitCoords = TopDownGrid->WorldToGrid(Unit->GetActorLocation());

		// Get unit sight
		const int UnitSight = TopDownGrid->ToGridUnit(Unit->GetSight());

		// 유닛을 기준으로 안개 텍스처 버퍼 업데이트
		FogTexture->UpdateFogBuffer(UnitCoords, UnitSight, TopDownGrid->IsBlocked);
	}
	// 버퍼로 텍스처 업데이트
	FogTexture->UpdateFogTexture();
}

void AFogManager::Client_UpdateUnitVisibility_Implementation()
{
	// Get TopDownPC
	auto TopDownPC = Cast<ATopDownPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(": Invalid TopDownPC"));
		return;
	}
	
	// TopDownPC가 소유하지 않은 유닛 중에서
	for (auto Unit : TopDownPC->OtherUnits)
	{
		// 그 유닛이 있는 곳의 안개 상태를 확인하고 유닛의 가시성을 결정합니다.
		auto Coords = TopDownGrid->WorldToGrid(Unit->GetActorLocation());
		if (FogTexture->IsRevealed(Coords))
		{
			Unit->SetUnitVisibility(true);
		}
		else
		{
			Unit->SetUnitVisibility(false);
		}
	}
}

UTexture2D* AFogManager::GetFogTexture() const
{
	return FogTexture->FogTexture;
}
