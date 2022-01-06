// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TopDownUnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTopDownUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FOGOFWAR_API ITopDownUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetSelect(bool NewSelect) = 0;
	virtual bool IsFriendly() const = 0;
	virtual bool IsHostile() const = 0;
};
