// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownHUD.h"
#include "TopDown/TopDownPlayerController.h"

void ATopDownHUD::DrawRectNoFill(const FVector2D& LeftTop, const FVector2D& RightBottom, const FLinearColor& Color)
{
	float LineThickness = FVector2D::DistSquared(LeftTop, RightBottom) > 20.0f * 20.0f ? 1.5f : 0.001f;

	// LeftTop to RightTop
	DrawLine(LeftTop.X, LeftTop.Y, RightBottom.X, LeftTop.Y, Color, LineThickness);

	// RightTop to RightBottom
	DrawLine(RightBottom.X, LeftTop.Y, RightBottom.X, RightBottom.Y, Color, LineThickness);

	// RightBottom to LeftBottom
	DrawLine(RightBottom.X, RightBottom.Y, LeftTop.X, RightBottom.Y, Color, LineThickness);

	// LeftBottom to LeftTop
	DrawLine(LeftTop.X, RightBottom.Y, LeftTop.X, LeftTop.Y, Color, LineThickness);
}
