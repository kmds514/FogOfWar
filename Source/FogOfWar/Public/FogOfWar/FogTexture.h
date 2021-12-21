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
	FFogTexture();
	~FFogTexture();

	void InitFogTexture(const uint32 Resolution);

	void ReleaseFogTexture();

	void UpdateFogTexture();

	void SetBuffer(const uint32 Index, const uint8 Value);

	UPROPERTY(Category = "Fog Texture", BlueprintReadOnly, Transient)
	class UTexture2D* FogTexture = nullptr;

private:
	uint8* Buffer = nullptr;
	uint32 BufferSize = 0;
	uint32 Width = 0;
	uint32 Height = 0;
	FUpdateTextureRegion2D UpdateRegion;
};
