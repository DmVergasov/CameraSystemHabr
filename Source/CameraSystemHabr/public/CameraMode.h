// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraMode.generated.h"

UCLASS(Abstract, Blueprintable, editinlinenew)
class CAMERASYSTEMHABR_API UCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float InterpolationSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float InterpolationLerpDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float ArmLength = 250.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float Fov = 60.f;

	UPROPERTY(EditDefaultsOnly)
	FVector CameraOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmPivotLocationOffset;

	UPROPERTY(EditDefaultsOnly)
	bool bUseControllerDesiredRotation = false;
	
};
