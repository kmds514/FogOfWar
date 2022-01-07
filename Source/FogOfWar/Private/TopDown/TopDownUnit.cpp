// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownUnit.h"
#include "TopDown/TopDownPlayerController.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ATopDownUnit::ATopDownUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Selection = CreateDefaultSubobject<UDecalComponent>("Selection Decal");
	Selection->SetupAttachment(RootComponent);
	Selection->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	Selection->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	Selection->SetRelativeLocation(FVector(0, 0, -90.0f));
	Selection->SetVisibility(false);
}

FGenericTeamId ATopDownUnit::GetGenericTeamId() const
{
	return (FGenericTeamId)TeamId;
}

void ATopDownUnit::SetSelect_Implementation(bool NewSelect)
{
	Selection->SetVisibility(NewSelect);
}

bool ATopDownUnit::IsFriendly_Implementation() const
{
	// Get TopDownPC
	auto TopDownPC = GetWorld()->GetFirstPlayerController<ATopDownPlayerController>();
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		return false;
	}

	// TopDownPC와 TeamId가 같으면 아군입니다.
	return TopDownPC->TeamId == TeamId;
}

bool ATopDownUnit::IsHostile_Implementation() const
{
	// Get TopDownPC
	auto TopDownPC = GetWorld()->GetFirstPlayerController<ATopDownPlayerController>();
	if (TopDownPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		return true;
	}

	// TopDownPC와 TeamId가 다르면 적군입니다.
	return TopDownPC->TeamId != TeamId;
}

void ATopDownUnit::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	// 유닛을 마우스 왼쪽 버튼으로 클릭하면
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		// Get TopDownPC
		auto TopDownPC = GetWorld()->GetFirstPlayerController<ATopDownPlayerController>();
		if (TopDownPC == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		}

		// TopDownPC가 현재 선택 중인 유닛을 선택해제합니다.
		TopDownPC->ClearSelectedActors();
		
		// TopDownPC와 같은 팀인지 확인합니다.
		if (ITopDownUnitInterface::Execute_IsHostile(this))
		{
			Client_SelectUnit(FLinearColor::Red);
		}
		if (ITopDownUnitInterface::Execute_IsFriendly(this))
		{
			Client_SelectUnit(FLinearColor::Green);
		}

		// TopDownPC가 선택 중인 유닛을 자신으로 갱신합니다.
		TopDownPC->SelectedActors.Add(this);
	}
}

void ATopDownUnit::Client_SelectUnit_Implementation(const FLinearColor& Color)
{
	SelectionDMI->SetVectorParameterValue(TEXT("Color"), Color);
	ITopDownUnitInterface::Execute_SetSelect(this, true);
}

int ATopDownUnit::GetSight() const
{
	return Sight;
}
