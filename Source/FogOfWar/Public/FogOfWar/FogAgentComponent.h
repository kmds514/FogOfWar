// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FogAgentComponent.generated.h"

UCLASS(ClassGroup = (FogOfWar), meta = (BlueprintSpawnableComponent))
class FOGOFWAR_API UFogAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFogAgentComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(Category = "Fog Agent", BlueprintPure)
	FVector GetFogAgentLocation() const;

	/** In centimeters */
	UPROPERTY(Category = "Fog Agent", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000"))
	int Sight = 600;

private:
	UPROPERTY()
	class AFogManager* FogManager = nullptr;
};
