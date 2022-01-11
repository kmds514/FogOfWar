// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Texture2DResource.h"

struct FTexel2X2
{
	bool operator==(const FTexel2X2& Rhs) const
	{
		return T[0] == Rhs.T[0] && T[1] == Rhs.T[1] && T[2] == Rhs.T[2] && T[3] == Rhs.T[3];
	}
	uint8 T[4];
};
uint32 GetTypeHash(const FTexel2X2& Texel); // TMap을 사용하기 위한 커스텀 해시 함수

struct FTexel4X4
{
	uint8 T[4][4];
};

class FOGOFWAR_API FFogTexture
{
	struct FUpdateTextureContext
	{
		FTexture2DResource* TextureResource = nullptr;
		uint32 MipIndex = 0;
		FUpdateTextureRegion2D* UpdateRegion = nullptr;
		uint32 SourcePitch = 0;
		uint8* SourceData = nullptr;
	};

	struct FOctantTransform
	{
		int8 XX, XY, YX, YY;
	};

	static constexpr uint8 RevealedFogColor = 0xFF;
	static constexpr uint8 ExploredFogColor = 0x20;

public:
	FFogTexture();
	~FFogTexture();

	void InitFogTexture(const uint32 Resolution);

	void ReleaseFogTexture();

	void UpdateExploredFog();

	void UpdateFogBuffer(const FIntPoint& Center, int Radius, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);

	void UpdateFogTexture();

	/** @return 타일이 Revealed 상태인지 알려줍니다. */
	UFUNCTION(Category = "Fog Texture", BlueprintPure)
	bool IsRevealed(const FIntPoint& Coords) const;

	/** 업스케일 버퍼로 만든 Texture2D */
	UPROPERTY(Category = "Fog Texture", BlueprintReadOnly, Transient)
	class UTexture2D* FogTexture = nullptr;

private:
	/** https://technology.riotgames.com/news/story-fog-and-war */
	void GenerateUpscaleMap();
	void UpdateUpscaleBuffer();

	// Ray casting: 중심에서 원 둘레에 해당하는 타일까지 직선을 그려 장애물을 검사하는 방식
	/** 브레젠험 원 알고리즘으로 원 둘에에 해당하는 타일을 구하는 함수
	* https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
	void DrawRayCastingFog(const FIntPoint& Center, int Radius, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);
	/** 브레젠험 선 알고리즘으로 직선상의 장애물 검사
	* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm */
	void CastBresenhamLine(const FIntPoint& Start, const FIntPoint& End, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);

	// Shadow casting
	/** http://roguebasin.com/index.php/Shadow_casting */
	void DrawShadowCastingFog(const FIntPoint& Center, int Radius, int Row, float Start, float End, const FOctantTransform& T, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);

	/** Target이 Center를 중심으로 하는 원 안에 있는지 확인합니다. */
	bool IsInRadius(const FIntPoint& Center, const FIntPoint& Target, int Radius) const;

	/** (X,Y), (X+1,Y), (X,Y+1), (X+1,Y+1) 텍셀을 가져옵니다. */
	FTexel2X2 GetTexel2X2(int X, int Y);

	/** 타일맵과 연동하여 안개 상태를 업데이트하는 버퍼입니다. */
	uint8* SourceBuffer = nullptr;
	uint32 SourceBufferSize = 0;
	int SourceWidth = 0;
	int SourceHeight = 0;

	/** SourceBuffer를 4배 확대한 버퍼입니다. */
	uint8* UpscaleBuffer = nullptr;
	uint32 UpscaleBufferSize = 0;
	int UpscaleWidth = 0;
	int UpscaleHeight = 0;
	FUpdateTextureRegion2D UpscaleUpdateRegion;

	/** 탐사했던 타일을 회색으로 만드는데 사용합니다. */
	uint8* ExploredBuffer = nullptr;

	/** 모든 8분면에 섀도 캐스팅 알고리즘을 적용하기 위한 변환 구조체입니다. */
	UPROPERTY()
	TArray<FOctantTransform> OctantTransforms;

	/** 2X2 텍셀을 4X4 텍셀로 어떻게 맵핑할지 저장해 놓은 템플릿입니다. */
	UPROPERTY()
	TMap<FTexel2X2, FTexel4X4> UpscaleMap;
};
