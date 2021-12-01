// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDown/TopDownCamera.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ATopDownCamera::ATopDownCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Handle = CreateDefaultSubobject<USpringArmComponent>("Handle");
	Handle->SetupAttachment(Collision);
	Handle->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Handle);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	TargetDistance = MaxDistance;
	TargetFOV = MaxFOV;
	TargetPitch = MaxPitch;
	TargetSpeed = MaxSpeed;

	Handle->TargetArmLength = TargetDistance;
	Camera->FieldOfView = TargetFOV;
	Handle->SetRelativeRotation(FRotator(-TargetPitch, 0.0f, 0.0f));
	Movement->MaxSpeed = TargetSpeed;
}

// Called when the game starts or when spawned
void ATopDownCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopDownCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopDownCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATopDownCamera::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATopDownCamera::OnMoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &ATopDownCamera::OnZoomCamera);
}

void ATopDownCamera::OnMoveForward(float Value)
{
	// Yaw 값만 저장
	FRotator Rotation(0.0f, Handle->GetRelativeRotation().Yaw, 0.0f);

	AddMovementInput(Rotation.Vector(), Value);
}

void ATopDownCamera::OnMoveRight(float Value)
{
	// Yaw 값만 저장
	FRotator Rotation(0.0f, Handle->GetRelativeRotation().Yaw, 0.0f);

	// 오른쪽 방향 벡터
	const FVector& Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(Right, Value);
}

void ATopDownCamera::OnZoomCamera(float Value)
{
	float AxisValue = Value;

	auto InterpValue = [&](float Min, float Max, float Current, float& Target)
	{
		float Strength = (Max - Min) / static_cast<float>(NumberOfZoomLevel) * AxisValue;

		Target = FMath::Clamp(Target - Strength, Min, Max);

		return FMath::FInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	};

	Handle->TargetArmLength = InterpValue(MinDistance, MaxDistance, Handle->TargetArmLength, TargetDistance);
	Camera->FieldOfView = InterpValue(MinFOV, MaxFOV, Camera->FieldOfView, TargetFOV);
	Movement->MaxSpeed = InterpValue(MinSpeed, MaxSpeed, Movement->MaxSpeed, TargetSpeed);

	// 각도 저장
	FRotator Rotation = Handle->GetRelativeRotation();

	// 각도 증감량 계산
	float PitchStrength = (MaxPitch - MinPitch) / static_cast<float>(NumberOfZoomLevel) * AxisValue;

	// 각도 제한
	TargetPitch = FMath::Clamp(TargetPitch - PitchStrength, MinPitch, MaxPitch);

	// 각도 선형보간
	Rotation.Pitch = -FMath::FInterpTo(-Rotation.Pitch, TargetPitch, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	Handle->SetRelativeRotation(Rotation);
}
