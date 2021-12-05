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
	/** The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed */
	virtual void DrawHUD() override;

	UFUNCTION(Category = "Top Down", BlueprintCallable)
	void BeginDrawRect();

	UFUNCTION(Category = "Top Down", BlueprintCallable)
	void EndDrawRect();

	/** @return Returns the TopDownPlayerController or nullptr if TopDownPlayerController is nullptr */
	UFUNCTION(Category = "Top Down", BlueprintPure)
	ATopDownPlayerController* GetTopDownPlayerController() const;

private:
	void DrawRectNoFill(const FLinearColor& LineColor, float LineThickness);

	UPROPERTY()
	ATopDownPlayerController* TopDownPlayerController = nullptr;

	UPROPERTY()
	TArray<AActor*> SelectedActors;

	FVector2D MouseBegin;
	FVector2D MouseEnd;

	bool bDrawing = false;
	bool bDragging = false;
};
