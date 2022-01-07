// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "TopDown/TopDownUnitInterface.h"
#include "TopDownUnit.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownUnit : public ACharacter, public IGenericTeamAgentInterface, public ITopDownUnitInterface
{
	GENERATED_BODY()

public:
	ATopDownUnit();

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual bool IsFriendly_Implementation() const override;
	virtual bool IsHostile_Implementation() const override;

	int GetSight() const;

private:
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 TeamId = 255;

	/** In centimeters */
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "100", ClampMax = "2000", UIMin = "100", UIMax = "2000"))
	int Sight = 800;
};
