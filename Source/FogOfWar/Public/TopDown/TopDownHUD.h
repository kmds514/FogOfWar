// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

class ATopDownPlayerController;

UCLASS()
class FOGOFWAR_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** @return Returns whether the rect can be drawn */
	UFUNCTION(Category = "Top Down", BlueprintCallable)
	bool DrawRectNoFill(const FVector2D& LeftTop, const FVector2D& RightBottom, const FLinearColor& Color, float LineThickness);

	/** Minimum rect size that can drawn */
	UPROPERTY(Category = "Top Down", EditAnywhere)
	float MinRectSize = 20.0f;
};
