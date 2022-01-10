// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownCamera.generated.h"

UCLASS()
class FOGOFWAR_API ATopDownCamera : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnZoomCamera(float Value);

	UFUNCTION(Category = "Top Down Camera", BlueprintPure)
	class UCapsuleComponent* GetCollisionComponent() const;

protected:
	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "200.0", ClampMax = "2000.0", UIMin = "200.0", UIMax = "2000.0"))
	float MinDistance = 800.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "2000.0", ClampMax = "5000.0", UIMin = "2000.0", UIMax = "5000.0"))
	float MaxDistance = 3800.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "30.0", ClampMax = "60.0", UIMin = "30.0", UIMax = "65.0"))
	float MinFOV = 40.0;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "60.0", ClampMax = "90.0", UIMin = "60.0", UIMax = "90.0"))
	float MaxFOV = 60.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "15.0", ClampMax = "60.0", UIMin = "15.0", UIMax = "60.0"))
	float MinPitch = 60.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "60.0", ClampMax = "85.0", UIMin = "60.0", UIMax = "85.0"))
	float MaxPitch = 65.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "400.0", ClampMax = "2000.0", UIMin = "400.0", UIMax = "2000.0"))
	float MinSpeed = 2000.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "2000.0", ClampMax = "4000.0", UIMin = "2000.0", UIMax = "4000.0"))
	float MaxSpeed = 3000.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float InterpSpeed = 10.0f;

	UPROPERTY(Category = "Top Down Camera", EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "2", ClampMax = "16", UIMin = "2", UIMax = "16"))
	int NumberOfZoomLevel = 5;

private:
	UPROPERTY(Category = "Top Down Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* Handle = nullptr;

	UPROPERTY(Category = "Top Down Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera = nullptr;

	FVector2D MousePosition;

	float TargetDistance = 0.0f;
	float TargetFOV = 0.0f;
	float TargetPitch = 0.0f;
	float TargetSpeed = 0.0f;
};
