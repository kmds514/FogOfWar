// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownHUD.h"
#include "TopDown/TopDownPlayerController.h"
#include "TopDown/TopDownUnitInterface.h"

void ATopDownHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		TopDownPlayerController = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	}
	if (TopDownPlayerController == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(__FUNCTION__) + TEXT(" - Invalid TopDownPC"));
		return;
	}
}

void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawing == false)
	{
		return;
	}

	if (TopDownPlayerController->GetMousePosition(MouseEnd.X, MouseEnd.Y))
	{
		if (DrawRectNoFill(MouseBegin, MouseEnd, FLinearColor::Green, 1.5f))
		{
			GetActorsInSelectionRectangle(MouseBegin, MouseEnd, SelectedActors, false);
		}
	}
}

void ATopDownHUD::BeginDrawRect()
{
	if (TopDownPlayerController->GetMousePosition(MouseBegin.X, MouseBegin.Y))
	{
		bDrawing = true;
	}
}

void ATopDownHUD::EndDrawRect()
{
	bDrawing = false;

	// 사각형 안에 유닛이 없으면 아무것도 하지 않습니다.
	if (SelectedActors.Num() < 1)
	{
		return;
	}

	// TopDownPC가 선택 중인 유닛을 선택 해제합니다.
	TopDownPlayerController->ClearSelectedActors();

	// 사각형 안에 있는 유닛에 대해
	for (auto Actor : SelectedActors)
	{
		// TopDownPC가 소유한 유닛이면
		if (TopDownPlayerController->IsOwningUnit(Actor))
		{
			// 그 유닛을 선택합니다.
			if (Actor->GetClass()->ImplementsInterface(UTopDownUnitInterface::StaticClass()))
			{
				ITopDownUnitInterface::Execute_SetSelect(Actor, true);
			}
			/*auto IUnit = Cast<ITopDownUnitInterface>(Actor);
			if (IUnit)
			{
				IUnit->Execute_SetSelect(IUnit, true);
			}*/
		}
	}

	// TopDownPC가 선택 중인 유닛을 갱신합니다.
	TopDownPlayerController->SelectedActors = SelectedActors;

	// 사각형 안에 있던 유닛을 비웁니다.
	SelectedActors.Empty(SelectedActors.GetSlack());
}

bool ATopDownHUD::DrawRectNoFill(const FVector2D& LeftTop, const FVector2D& RightBottom, const FLinearColor& Color, float LineThickness)
{
	// 사각형이 너무 작으면 그리지 않습니다.
	if (FVector2D::DistSquared(LeftTop, RightBottom) <= MinRectSize * MinRectSize)
	{
		return false;
	}

	// LeftTop to RightTop
	DrawLine(LeftTop.X, LeftTop.Y, RightBottom.X, LeftTop.Y, Color, LineThickness);

	// RightTop to RightBottom
	DrawLine(RightBottom.X, LeftTop.Y, RightBottom.X, RightBottom.Y, Color, LineThickness);

	// RightBottom to LeftBottom
	DrawLine(RightBottom.X, RightBottom.Y, LeftTop.X, RightBottom.Y, Color, LineThickness);

	// LeftBottom to LeftTop
	DrawLine(LeftTop.X, RightBottom.Y, LeftTop.X, LeftTop.Y, Color, LineThickness);

	return true;
}
