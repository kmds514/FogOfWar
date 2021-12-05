// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

UENUM(BlueprintType)
enum class EEdgeLocation : uint8
{
	None, Top, Bottom, Left, Right, LeftTop, RightTop, LeftBottom, RightBottom
};

UCLASS()
class FOGOFWAR_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Top Down", BlueprintPure)
	EEdgeLocation GetMouseEdgeLocation() const;

private:
	EEdgeLocation CheckEdgeMovement();

	UPROPERTY()
	class ATopDownCamera* TopDownCamera = nullptr;
	FVector2D MouseLocation;
	FVector2D ViewportSize;

	EEdgeLocation EdgeLocation = EEdgeLocation::None;
};
