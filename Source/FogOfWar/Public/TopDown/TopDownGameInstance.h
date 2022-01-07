// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TopDownGameInstance.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TeamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;
};

UCLASS()
class FOGOFWAR_API UTopDownGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Category = "Top Down Game Instance", BlueprintPure)
	FText GetUserName();

	UFUNCTION(Category = "Top Down Game Instance", BlueprintCallable)
	void SetPlayerInfo(const FText& UserName, int TeamId, const FLinearColor& TeamColor);

	UPROPERTY(Category = "Top Down Game Instance", BlueprintReadWrite)
	FPlayerInfo PlayerInfo = {};
};
