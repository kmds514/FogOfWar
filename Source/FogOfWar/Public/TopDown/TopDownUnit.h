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
	// Sets default values for this character's properties
	ATopDownUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	int GetSight() const;

protected:
	UPROPERTY(Category = "Top Down Unit", EditAnywhere, BlueprintReadWrite)
	uint8 TeamId = 255;

	/** In centimeters */
	UPROPERTY(Category = "Top Down Unit", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	int Sight = 800;

	UPROPERTY(Category = "Top Down Unit", VisibleAnywhere, BlueprintReadOnly)
	class UFogAgentComponent* FogAgent = nullptr;
};
