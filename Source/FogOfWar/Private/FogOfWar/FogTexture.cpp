// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogTexture.h"
#include "Engine/Texture2D.h"

FFogTexture::FFogTexture()
{
	OctantTransforms.Reserve(8);
	OctantTransforms.Emplace(FOctantTransform{  1,  0,  0,  1 });
	OctantTransforms.Emplace(FOctantTransform{  1,  0,  0, -1 });
	OctantTransforms.Emplace(FOctantTransform{ -1,  0,  0,  1 });
	OctantTransforms.Emplace(FOctantTransform{ -1,  0,  0, -1 });
	OctantTransforms.Emplace(FOctantTransform{  0,  1,  1,  0 });
	OctantTransforms.Emplace(FOctantTransform{  0,  1, -1,  0 });
	OctantTransforms.Emplace(FOctantTransform{  0, -1,  1,  0 });
	OctantTransforms.Emplace(FOctantTransform{  0, -1, -1,  0 });
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
	FogTexture->MipGenSettings = TextureMipGenSettings::TMGS_Blur5;
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

void FFogTexture::CalculateFog(const FIntPoint& Center, int Radius, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked)
{
	Buffer[Center.Y * Width + Center.X] = 0xFF;

	for (int i = 0; i < 8; ++i)
	{
		CastShadow(Center, Radius, 1, 1.0f, 0.0f, OctantTransforms[i], IsBlocked);
	}
}

void FFogTexture::CastShadow(const FIntPoint& Center, int Radius, int Row, float Start, float End, const FOctantTransform& T, TFunction<bool(const FIntPoint&, const FIntPoint&)> IsBlocked)
{
	if (Start < End)
	{
		return;
	}

	float NewStart = 0.0f;
	bool bBlocked = false;

	for (int Distance = Row; Distance <= Radius && bBlocked == false; ++Distance)
	{
		int DeltaY = -Distance;
		for (int DeltaX = -Distance; DeltaX <= 0; ++DeltaX)
		{
			int CurrentX = Center.X + DeltaX * T.XX + DeltaY * T.XY;
			int CurrentY = Center.Y + DeltaX * T.YX + DeltaY * T.YY;
			float LeftSlope = (DeltaX - 0.5f) / (DeltaY + 0.5f);
			float RightSlope = (DeltaX + 0.5f) / (DeltaY - 0.5f);

			if (((CurrentX >= 0 && CurrentY >= 0 && CurrentX < Width && CurrentY < Height) == false) || Start < RightSlope)
			{
				continue;
			}
			else if (End > LeftSlope)
			{
				break;
			}

			if (IsInRadius(Center, { CurrentX, CurrentY }, Radius))
			{
				Buffer[CurrentY * Width + CurrentX] = 0xFF;
			}

			if (bBlocked)
			{
				if (IsBlocked(Center, { CurrentX, CurrentY }))
				{
					NewStart = RightSlope;
					continue;
				}
				else
				{
					bBlocked = false;
					Start = NewStart;
				}
			}
			else
			{
				if (IsBlocked(Center, { CurrentX, CurrentY }) && Distance < Radius)
				{
					bBlocked = true;
					CastShadow(Center, Radius, Distance + 1, Start, LeftSlope, T, IsBlocked);
					NewStart = RightSlope;
				}
			}
		}
	}
}

void FFogTexture::ResetBuffer()
{
	FMemory::Memset(Buffer, 0x04, BufferSize);
}

bool FFogTexture::IsInRadius(const FIntPoint& Center, const FIntPoint& Target, int Radius) const
{
	return (Target.X - Center.X) * (Target.X - Center.X) + (Target.Y - Center.Y) * (Target.Y - Center.Y) < Radius * Radius;
}

void FFogTexture::SetBuffer(const uint32 Index, const uint8 Value)
{
	Buffer[Index] = Value;
}
