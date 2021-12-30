// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGameState.h"
#include "TopDown/TopDownUnit.h"
#include "Kismet/GameplayStatics.h"

void ATopDownGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownUnit::StaticClass(), OutActors);

	for (auto Actor : OutActors)
	{
		AllUnits.Add(Cast<ATopDownUnit>(Actor));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::FromInt(AllUnits.Num()));
}

void ATopDownGameState::AddUnit(ATopDownUnit* const Unit)
{
	AllUnits.Add(Unit);
}

void ATopDownGameState::RemoveUnit(ATopDownUnit* const Unit)
{
	AllUnits.Remove(Unit);
}
