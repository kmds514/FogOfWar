// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TopDownGameState.generated.h"

class ATopDownUnit;

UCLASS()
class FOGOFWAR_API ATopDownGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Category = "Top Down Game State", BlueprintCallable)
	void AddUnit(ATopDownUnit* const Unit);

	UFUNCTION(Category = "Top Down Game State", BlueprintCallable)
	void RemoveUnit(ATopDownUnit* const Unit);

	UPROPERTY(Category = "Top Down Game State", BlueprintReadOnly)
	TArray<ATopDownUnit*> AllUnits;
};
