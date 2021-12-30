// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownUnit.h"
#include "FogOfWar/FogAgentComponent.h"

// Sets default values
ATopDownUnit::ATopDownUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FogAgent = CreateDefaultSubobject<UFogAgentComponent>(TEXT("Fog Agent"));
	FogAgent->Sight = Sight;
}

// Called when the game starts or when spawned
void ATopDownUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopDownUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FGenericTeamId ATopDownUnit::GetGenericTeamId() const
{
	return (FGenericTeamId)TeamId;
}

int ATopDownUnit::GetSight() const
{
	return Sight;
}

