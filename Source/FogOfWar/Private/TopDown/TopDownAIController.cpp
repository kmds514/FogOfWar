// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownAIController.h"
#include "TopDown/TopDownUnit.h"

void ATopDownAIController::BeginPlay()
{
    Super::BeginPlay();

    Unit = GetPawn<ATopDownUnit>();
    if (Unit)
    {
        TeamId = Unit->GetGenericTeamId();
    }
}

ETeamAttitude::Type ATopDownAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    // 액터가 폰인지 확인합니다.
    const APawn* OtherPawn = Cast<APawn>(&Other);
    if (OtherPawn == nullptr)
    {
        return ETeamAttitude::Neutral;
    }

    // 액터가 IGenericTeamAgentInterface를 구현했는지 확인합니다.
    const IGenericTeamAgentInterface* IBot = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
    if (IBot == nullptr)
    {
        return ETeamAttitude::Neutral;
    }

    // 액터의 팀 아이디를 가져옵니다.
    FGenericTeamId OtherTeamId = IBot->GetGenericTeamId();

    // OtherActor에 대한 태도를 결정합니다.
    if (TeamId == OtherTeamId)
    {
        return ETeamAttitude::Friendly;
    }
    else if (OtherTeamId == 255)
    {
        return ETeamAttitude::Neutral;
    }
    else
    {
        return ETeamAttitude::Hostile;
    }
}

ETeamAttitude::Type ATopDownAIController::GetAttitudeTowards(const AActor* Other) const
{
    return GetTeamAttitudeTowards(*Other);
}

FGenericTeamId ATopDownAIController::GetGenericTeamId() const
{
    return TeamId;
}
