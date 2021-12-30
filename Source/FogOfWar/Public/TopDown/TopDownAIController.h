// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopDownAIController.generated.h"

/**
 * 
 */
UCLASS()
class FOGOFWAR_API ATopDownAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	UFUNCTION(Category = "Top Down AI", BlueprintPure)
	ETeamAttitude::Type GetAttitudeTowards(const AActor* Other) const;

protected:
	UPROPERTY(Category = "Top Down AI", BlueprintReadOnly)
	class ATopDownUnit* Unit = nullptr;

	FGenericTeamId TeamId;
};
