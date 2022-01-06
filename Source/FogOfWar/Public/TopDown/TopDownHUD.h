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

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	void BeginDrawRect();
	void EndDrawRect();

	/** @return Returns whether the rect can be drawn */
	UFUNCTION(Category = "Top Down", BlueprintCallable)
	bool DrawRectNoFill(const FVector2D& LeftTop, const FVector2D& RightBottom, const FLinearColor& Color, float LineThickness);

private:
	UPROPERTY()
	ATopDownPlayerController* TopDownPlayerController = nullptr;

	UPROPERTY()
	TArray<AActor*> SelectedActors;

	FVector2D MouseBegin;
	FVector2D MouseEnd;

	uint8 bDrawing : 1;

	/** Minimum rect size that can drawn */
	float MinRectSize = 20.0f;
};
