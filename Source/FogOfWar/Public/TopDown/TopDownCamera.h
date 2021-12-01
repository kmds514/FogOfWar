// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopDownCamera.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopDownCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnZoomCamera(float Value);

private:
	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Collision = nullptr;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* Handle = nullptr;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* Movement = nullptr;

	FVector2D MousePosition;

protected:
	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "200.0", ClampMax = "1000.0", UIMin = "200.0", UIMax = "1000.0"))
	float MinDistance = 300.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1000.0", ClampMax = "2000.0", UIMin = "1000.0", UIMax = "2000.0"))
	float MaxDistance = 1200.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "50.0", ClampMax = "80.0", UIMin = "50.0", UIMax = "80.0"))
	float MinFOV = 60.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "80.0", ClampMax = "100.0", UIMin = "80.0", UIMax = "100.0"))
	float MaxFOV = 90.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "5.0", ClampMax = "45.0", UIMin = "5.0", UIMax = "45.0"))
	float MinPitch = 15.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "45.0", ClampMax = "85.0", UIMin = "45.0", UIMax = "85.0"))
	float MaxPitch = 65.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "200.0", ClampMax = "1000.0", UIMin = "200.0", UIMax = "1000.0"))
	float MinSpeed = 600.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1000.0", ClampMax = "3000.0", UIMin = "1000.0", UIMax = "3000.0"))
	float MaxSpeed = 2400.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float InterpSpeed = 10.0f;

	UPROPERTY(Category = "Camera Zoom", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "10", UIMin = "1", UIMax = "10"))
	int NumberOfZoomLevel = 5;

private:
	float TargetDistance = 0.0f;
	float TargetFOV = 0.0f;
	float TargetPitch = 0.0f;
	float TargetSpeed = 0.0f;
};
