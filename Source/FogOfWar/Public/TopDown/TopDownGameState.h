// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TopDownGameState.generated.h"

class ATopDownUnit;

UCLASS()
class FOGOFWAR_API ATopDownGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Category = "Top Down Game State", BlueprintReadOnly, Replicated)
	TArray<ATopDownUnit*> AllUnits;
};
