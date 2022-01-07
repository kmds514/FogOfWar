// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownUnit.h"
#include "TopDown/TopDownPlayerController.h"

// Sets default values
ATopDownUnit::ATopDownUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

FGenericTeamId ATopDownUnit::GetGenericTeamId() const
{
	return (FGenericTeamId)TeamId;
}

bool ATopDownUnit::IsFriendly_Implementation() const
{
	// Get TopDownPC
	auto TopDownPC = GetWorld()->GetFirstPlayerController<ATopDownPlayerController>();
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		return false;
	}

	// TopDownPC와 TeamId가 같으면 아군입니다.
	return TopDownPC->TeamId == TeamId;
}

bool ATopDownUnit::IsHostile_Implementation() const
{
	// Get TopDownPC
	auto TopDownPC = GetWorld()->GetFirstPlayerController<ATopDownPlayerController>();
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		return true;
	}

	// TopDownPC와 TeamId가 다르면 적군입니다.
	return TopDownPC->TeamId != TeamId;
}

int ATopDownUnit::GetSight() const
{
	return Sight;
}
