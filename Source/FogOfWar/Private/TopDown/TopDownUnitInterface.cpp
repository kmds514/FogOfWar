// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownUnitInterface.h"

// Add default functionality here for any ITopDownUnitInterface functions that are not pure virtual.

void ITopDownUnitInterface::SetSelect_Implementation(bool NewSelect)
{
}

bool ITopDownUnitInterface::IsFriendly_Implementation() const
{
    return false;
}

bool ITopDownUnitInterface::IsHostile_Implementation() const
{
    return false;
}
