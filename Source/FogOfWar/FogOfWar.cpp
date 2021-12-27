// Fill out your copyright notice in the Description page of Project Settings.

#include "FogOfWar.h"
#include "Modules/ModuleManager.h"

void FFogOfWarModule::StartupModule()
{
	FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping("/Project", ShaderDirectory);
}

IMPLEMENT_PRIMARY_GAME_MODULE( FFogOfWarModule, FogOfWar, "FogOfWar" );
