// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogTexture.h"
#include "Engine/Texture2D.h"

FFogTexture::FFogTexture()
{
}

FFogTexture::~FFogTexture()
{
	ReleaseFogTexture();
}

void FFogTexture::InitFogTexture(const uint32 Resolution)
{
	ReleaseFogTexture();

	uint32 TextureSize = FMath::RoundUpToPowerOfTwo(Resolution);

	Width = TextureSize;
	Height = TextureSize;
	Buffer = new uint8[Width * Height];
	BufferSize = Width * Height * sizeof(uint8);
	FMemory::Memset(Buffer, 0x00, BufferSize);
	UpdateRegion = FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);

	FogTexture = UTexture2D::CreateTransient(Width, Height, EPixelFormat::PF_G8);
	FogTexture->Filter = TextureFilter::TF_Nearest;
	FogTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
	FogTexture->AddressX = TextureAddress::TA_Clamp;
	FogTexture->AddressY = TextureAddress::TA_Clamp;
	FogTexture->SRGB = false;
	FogTexture->UpdateResource();
}

void FFogTexture::ReleaseFogTexture()
{
	if (FogTexture)
	{
		FogTexture = nullptr;
	}
	if (Buffer)
	{
		delete[] Buffer;
		Buffer = nullptr;
	}
}

void FFogTexture::UpdateFogTexture()
{
	uint8* FogData = new uint8[BufferSize];
	FMemory::Memcpy(FogData, Buffer, BufferSize);

	FFogTextureContext* FogTextureContext = new FFogTextureContext();
	FogTextureContext->TextureResource = (FTexture2DResource*)FogTexture->Resource;
	FogTextureContext->MipIndex = FogTextureContext->TextureResource->GetCurrentFirstMip();
	FogTextureContext->UpdateRegion = &UpdateRegion;
	FogTextureContext->SourcePitch = UpdateRegion.Width;
	FogTextureContext->SourceData = FogData;

	ENQUEUE_RENDER_COMMAND(UpdateTexture)([FogTextureContext](FRHICommandListImmediate& RHICmdList)
		{
			RHIUpdateTexture2D(FogTextureContext->TextureResource->GetTexture2DRHI(),
				FogTextureContext->MipIndex,
				*FogTextureContext->UpdateRegion,
				FogTextureContext->SourcePitch,
				FogTextureContext->SourceData);
		});
}

void FFogTexture::SetBuffer(const uint32 Index, const uint8 Value)
{
	Buffer[Index] = Value;
}
