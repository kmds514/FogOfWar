// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Category = "Top Down Game Mode", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<APlayerController*> AllPlayerController;
};
