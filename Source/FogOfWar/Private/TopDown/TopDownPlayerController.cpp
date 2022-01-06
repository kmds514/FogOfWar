// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownPlayerController.h"
#include "TopDown/TopDownCamera.h"
#include "TopDown/TopDownHUD.h"
#include "TopDown/TopDownUnit.h"
#include "TopDown/TopDownGameState.h"
#include "Kismet/GameplayStatics.h"

void ATopDownPlayerController::BeginPlay()
{
    Super::BeginPlay();

	TopDownCamera = GetPawn<ATopDownCamera>();

	FInputModeGameAndUI InputMode = {};
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);

	// Get TopDownGS
	auto TopDownGS = Cast<ATopDownGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (TopDownGS == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Invalid TopDownGS"));
		return;
	}

	// 모든 TopDownUnit 순회
	for (auto Unit : TopDownGS->AllUnits)
	{
		// PC와 같은 팀 유닛만 OwningUnits에 추가
		if (Unit->GetGenericTeamId() == TeamId)
		{
			OwningUnits.Add(Unit);
		}
	}
}

void ATopDownPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TopDownCamera)
    {
        EdgeLocation = CheckEdgeMovement();
    }
}

void ATopDownPlayerController::AddOwningUnit(ATopDownUnit* const Unit)
{
	OwningUnits.Add(Unit);
}

void ATopDownPlayerController::RemoveOwningUnit(ATopDownUnit* const Unit)
{
	OwningUnits.Remove(Unit);
}

EEdgeLocation ATopDownPlayerController::GetMouseEdgeLocation() const
{
    return EdgeLocation;
}

bool ATopDownPlayerController::IsOwningUnit(AActor* const Actor) const
{
	auto Unit = Cast<ATopDownUnit>(Actor);
	if (Unit)
	{
		return Unit->GetGenericTeamId() == TeamId;
	}
	return false;
}

EEdgeLocation ATopDownPlayerController::CheckEdgeMovement()
{
	if (GetMousePosition(MouseLocation.X, MouseLocation.Y))
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		// 마우스 좌표 정규화
		MouseLocation /= ViewportSize;

		// Left Top
		if (MouseLocation.X < 0.02f && MouseLocation.Y < 0.02f)
		{
			TopDownCamera->OnMoveRight(-1.0f);
			TopDownCamera->OnMoveForward(1.0f);
			return EEdgeLocation::LeftTop;
		}
		// Right Top
		else if (MouseLocation.X > 0.98f && MouseLocation.Y < 0.02f)
		{
			TopDownCamera->OnMoveRight(1.0f);
			TopDownCamera->OnMoveForward(1.0f);
			return EEdgeLocation::RightTop;
		}
		// Left Bottom
		else if (MouseLocation.X < 0.02f && MouseLocation.Y > 0.98f)
		{
			TopDownCamera->OnMoveRight(-1.0f);
			TopDownCamera->OnMoveForward(-1.0f);
			return EEdgeLocation::LeftBottom;
		}
		// Right Bottom
		else if (MouseLocation.X > 0.98f && MouseLocation.Y > 0.98f)
		{
			TopDownCamera->OnMoveRight(1.0f);
			TopDownCamera->OnMoveForward(-1.0f);
			return EEdgeLocation::RightBottom;
		}
		// Right
		else if (MouseLocation.X > 0.98f)
		{
			TopDownCamera->OnMoveRight(1.0f);
			return EEdgeLocation::Right;
		}
		// Left
		else if (MouseLocation.X < 0.02f)
		{
			TopDownCamera->OnMoveRight(-1.0f);
			return EEdgeLocation::Left;
		}
		// Bottom
		else if (MouseLocation.Y > 0.98f)
		{
			TopDownCamera->OnMoveForward(-1.0f);
			return EEdgeLocation::Bottom;
		}
		// Top
		else if (MouseLocation.Y < 0.02f)
		{
			TopDownCamera->OnMoveForward(1.0f);
			return EEdgeLocation::Top;
		}
	}
	return EEdgeLocation::None;
}
