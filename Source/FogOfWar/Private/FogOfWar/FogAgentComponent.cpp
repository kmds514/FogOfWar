// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogAgentComponent.h"
#include "FogOfWar/FogManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFogAgentComponent::UFogAgentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UFogAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFogManager::StaticClass(), OutActors);
	if (OutActors.Num() == 1)
	{
		FogManager = Cast<AFogManager>(OutActors[0]);

		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Log, TEXT("%s: Successfully get FogManager."), *Name);
	}
	else
	{
		FString Name;
		GetName(Name);
		UE_LOG(LogTemp, Error, TEXT("%s: FogManager must exist only one instance in world. Current instance is %d "), *Name, OutActors.Num());
	}

	if (FogManager)
	{
		FogManager->AddFogAgent(this);
	}
}

void UFogAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (FogManager)
	{
		FogManager->RemoveFogAgent(this);
	}
}

FVector UFogAgentComponent::GetFogAgentLocation() const
{
	return GetOwner()->GetActorLocation();
}
