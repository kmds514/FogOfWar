// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "TopDownUnit.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownUnit : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATopDownUnit();

	virtual FGenericTeamId GetGenericTeamId() const override;

	/** In centimeters */
	UPROPERTY(Category = "Top Down Unit", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	int Sight = 800;

protected:
	UPROPERTY(Category = "Top Down Unit", EditAnywhere, BlueprintReadWrite)
	uint8 TeamId = 255;
};
