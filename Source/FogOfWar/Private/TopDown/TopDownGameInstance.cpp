// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownGameInstance.h"

FText UTopDownGameInstance::GetUserName()
{
	if (PlayerInfo.Name.IsEmpty())
	{
		int32 N = FMath::RandHelper(1000);
		FString NewName = (FString("User_") + FString::FromInt(N));
		PlayerInfo.Name = FText::FromString(NewName);
	}
	return PlayerInfo.Name;
}

void UTopDownGameInstance::SetPlayerInfo(const FText& UserName, int TeamId, const FLinearColor& TeamColor)
{
	PlayerInfo.Name = UserName;
	PlayerInfo.TeamId = TeamId;
	PlayerInfo.Color = TeamColor;
}
