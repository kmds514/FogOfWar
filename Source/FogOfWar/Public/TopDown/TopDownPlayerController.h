// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class ATopDownUnit;

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
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void OnLeftMouseButtonPressed();
	void OnLeftMouseButtonReleased();

	UFUNCTION(Category = "Top Down Player Controller", BlueprintCallable)
	void ClearSelectedActors();

	UFUNCTION(Category = "Top Down Player Controller", BlueprintPure)
	EEdgeLocation GetMouseEdgeLocation() const;

	UFUNCTION(Category = "Top Down Player Controller", BlueprintPure)
	bool IsOwningUnit(AActor* const Unit) const;

public:
	UPROPERTY(Category = "Top Down Player Controller", VisibleAnywhere, BlueprintReadWrite)
	uint8 TeamId = 0;

	UPROPERTY(Category = "Top Down Player Controller", BlueprintReadOnly)
	TArray<ATopDownUnit*> OwningUnits;
	UPROPERTY(Category = "Top Down Player Controller", BlueprintReadOnly)
	TArray<ATopDownUnit*> OtherUnits;

	UPROPERTY(Category = "Top Down Player Controller", BlueprintReadOnly)
	TArray<AActor*> SelectedActors;

private:
	EEdgeLocation CheckEdgeMovement();

	UPROPERTY(Category = "Top Down Player Controller", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATopDownCamera* TopDownCamera = nullptr;

	FVector2D MouseLocation;
	FVector2D ViewportSize;
	EEdgeLocation EdgeLocation = EEdgeLocation::None;
};
