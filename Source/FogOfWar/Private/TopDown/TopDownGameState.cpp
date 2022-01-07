// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGameState.h"
#include "TopDown/TopDownUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ATopDownGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownUnit::StaticClass(), OutActors);
		for (auto Actor : OutActors)
		{
			AllUnits.Add(Cast<ATopDownUnit>(Actor));
		}
	}
}

void ATopDownGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATopDownGameState, AllUnits);
}
