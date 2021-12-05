// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownHUD.h"
#include "TopDown/TopDownPlayerController.h"

void ATopDownHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		TopDownPlayerController = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	}
}

void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawing)
	{
		GetTopDownPlayerController()->GetMousePosition(MouseEnd.X, MouseEnd.Y);

		bDragging = true;

		DrawRectNoFill(FLinearColor::Green, [&]()
			{
				// If the rect is too small, line thickness is zero
				return FVector2D::DistSquared(MouseBegin, MouseEnd) > 20.0f * 20.0f ? 1.5f : 0.001f;
			}());

		// 배열이 추가될 때마다 매번 재할당을 피하기 위해 미리 넉넉한 메모리를 제공하여 재할당에 드는 퍼포먼스 비용을 줄입니다. @see https://docs.unrealengine.com/4.27/ko/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/
		SelectedActors.Reset(SelectedActors.GetSlack());
		GetActorsInSelectionRectangle<AActor>(MouseBegin, MouseEnd, SelectedActors, false);
	}
}

void ATopDownHUD::BeginDrawRect()
{
	if (GetTopDownPlayerController()->GetMousePosition(MouseBegin.X, MouseBegin.Y))
	{
		bDrawing = true;
	}
}

void ATopDownHUD::EndDrawRect()
{
	bDrawing = false;

	if (bDragging)
	{
		// Sends Actors to TopDownPlayerController...
	}
	SelectedActors.Empty();
}

void ATopDownHUD::DrawRectNoFill(const FLinearColor& LineColor, float LineThickness)
{
	// LeftTop to RightTop
	DrawLine(MouseBegin.X, MouseBegin.Y, MouseEnd.X, MouseBegin.Y, LineColor, LineThickness);

	// RightTop to RightBottom
	DrawLine(MouseEnd.X, MouseBegin.Y, MouseEnd.X, MouseEnd.Y, LineColor, LineThickness);

	// RightBottom to LeftBottom
	DrawLine(MouseEnd.X, MouseEnd.Y, MouseBegin.X, MouseEnd.Y, LineColor, LineThickness);

	// LeftBottom to LeftTop
	DrawLine(MouseBegin.X, MouseEnd.Y, MouseBegin.X, MouseBegin.Y, LineColor, LineThickness);
}

ATopDownPlayerController* ATopDownHUD::GetTopDownPlayerController() const
{
	return TopDownPlayerController ? TopDownPlayerController : nullptr;
}
