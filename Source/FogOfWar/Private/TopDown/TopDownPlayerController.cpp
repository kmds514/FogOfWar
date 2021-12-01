// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownPlayerController.h"
#include "TopDown/TopDownCamera.h"

void ATopDownPlayerController::BeginPlay()
{
    Super::BeginPlay();

	TopDownCamera = GetPawn<ATopDownCamera>();
    EdgeLocation = EEdgeLocation::None;
}

void ATopDownPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TopDownCamera)
    {
        EdgeLocation = CheckEdgeMovement();
    }
}

EEdgeLocation ATopDownPlayerController::GetMouseEdgeLocation() const
{
    return EdgeLocation;
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
