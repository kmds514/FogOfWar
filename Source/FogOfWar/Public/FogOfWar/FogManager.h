// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDown/TopDownGrid.h"
#include "FogManager.generated.h"

class FFogTexture;

UCLASS()
class FOGOFWAR_API AFogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AFogManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(Category = "Fog Manager", BlueprintPure)
	UTexture2D* GetFogTexture() const;

protected:
	void UpdateFog();
	void UpdateFogTexture();

	UFUNCTION(Client, Reliable)
	void Client_UpdateUnitVisibility();
	void Client_UpdateUnitVisibility_Implementation();

	UPROPERTY(Category = "Fog Manager", BlueprintReadOnly)
	class ATopDownGrid* TopDownGrid = nullptr;

	/** Number of fog updates per second */
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax ="60", UIMin = "1", UIMax = "60"))
	uint8 FogUpdateInterval = 10;

	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "5", ClampMax = "30", UIMin = "5", UIMax = "30"))
	uint8 FogInterpSpeed = 20;

	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.001", ClampMax = "0.05", UIMin = "0.001", UIMax = "0.05"))
	float FogBlurDistance = 0.02f;

	/** 그리드의 각 축에 있는 타일 개수 & 안개 텍스처 해상도 */
	uint32 GridResolution = 0;

	FTimerHandle FogUpdateTimer;

	FFogTexture* FogTexture = nullptr;
};
