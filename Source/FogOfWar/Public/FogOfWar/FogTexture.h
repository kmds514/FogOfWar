// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Texture2DResource.h"

class FOGOFWAR_API FFogTexture
{
	struct FFogTextureContext
	{
		FTexture2DResource* TextureResource = nullptr;
		uint32 MipIndex = 0;
		FUpdateTextureRegion2D* UpdateRegion = nullptr;
		uint32 SourcePitch = 0;
		uint8* SourceData = nullptr;
	};

public:
	struct FOctantTransform
	{
		int8 XX, XY, YX, YY;
	};

	FFogTexture();
	~FFogTexture();

	void InitFogTexture(const uint32 Resolution);

	void ReleaseFogTexture();

	void UpdateFogTexture();

	void CalculateFog(const FIntPoint& Center, int Radius, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);

	/** http://www.roguebasin.com/index.php?title=Improved_Shadowcasting_in_Java */
	void CastShadow(const FIntPoint& Center, int Radius, int Row, float Start, float End, const FOctantTransform& T, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked);

	void ResetBuffer();

	void SetBuffer(const uint32 Index, const uint8 Value);

	UPROPERTY(Category = "Fog Texture", BlueprintReadOnly, Transient)
	class UTexture2D* FogTexture = nullptr;

private:
	bool IsInRadius(const FIntPoint& Center, const FIntPoint& Target, int Radius) const;

	uint8* Buffer = nullptr;
	uint32 BufferSize = 0;
	int Width = 0;
	int Height = 0;
	FUpdateTextureRegion2D UpdateRegion;

	TArray<FOctantTransform> OctantTransforms;
};
