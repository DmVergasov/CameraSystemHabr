// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraModeComponent.h"
#include "CameraMode.h"
#include "CameraSystemCharacter.h"
#include "Algo/Find.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

UCameraModeComponent::UCameraModeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraModeComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACameraSystemCharacter>(GetOwner());

	if (Character->IsLocallyControlled())
	{
		Character->OnTagContainerChanged.AddUObject(this, &UCameraModeComponent::OnAbilityTagChanged);
	}

	OriginSpringArmRelativeLocation = Character->GetCameraBoom()->GetRelativeLocation();

	PlayerCameraManager = CastChecked<APlayerController>(Character->GetController())->PlayerCameraManager;

	TryUpdateCameraMode();
}

void UCameraModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCameraMode(DeltaTime);
}

void UCameraModeComponent::OnAbilityTagChanged(const FGameplayTag& Tag, bool TagExists)
{
	TryUpdateCameraMode();
}

void UCameraModeComponent::TryUpdateCameraMode()
{
	SetCameraMode(DetermineCameraMode(Character->GetGameplayTags()));
}

void UCameraModeComponent::SetCameraMode(UCameraMode* NewMode)
{
	if (CurrentCameraMode != NewMode)
	{
		PreviousInterpSpeed = CurrentCameraMode == nullptr ? NewMode->InterpolationSpeed : CurrentCameraMode->InterpolationSpeed;

		CurrentCameraMode = NewMode;
		
		Character->GetCameraBoom()->bUsePawnControlRotation = CurrentCameraMode->bUsePawnControlRotation;
		TimeSecondsAfterSetNewMode = GetWorld()->GetTimeSeconds();
	}
}

UCameraMode* UCameraModeComponent::DetermineCameraMode(const FGameplayTagContainer& Tags) const
{
	if (auto foundMode = Algo::FindByPredicate(CameraModes, [&](const auto modeData) {return modeData.CanActivateMode(Tags); }))
	{
		return foundMode->GetCameraMode();
	}

	return nullptr;
}

float UCameraModeComponent::GetInterpSpeed() const
{
	auto timeAfterModeWasChanged = GetWorld()->GetTimeSeconds() - TimeSecondsAfterSetNewMode;
	auto lerpDuration = CurrentCameraMode->InterpolationLerpDuration;
	auto lerpAlpha = FMath::Min(1.f, timeAfterModeWasChanged / lerpDuration);
	return FMath::Lerp(PreviousInterpSpeed, CurrentCameraMode->InterpolationSpeed, lerpAlpha);
}

void UCameraModeComponent::UpdateCameraMode(float DeltaTime)
{
	if (CurrentCameraMode != nullptr)
	{
		UpdateSpringArmLength(DeltaTime);
		UpdateSpringArmPivotLocation(DeltaTime);
		UpdateCameraLocation(DeltaTime);
		UpdateFOV(DeltaTime);
	}
}

void UCameraModeComponent::UpdateSpringArmLength(float DeltaTime)
{
	const auto currentLength = Character->GetCameraBoom()->TargetArmLength;

	const auto targetLength = CurrentCameraMode->ArmLength;

	const auto newArmLength = FMath::FInterpTo(currentLength, targetLength, DeltaTime, GetInterpSpeed());

	Character->GetCameraBoom()->TargetArmLength = newArmLength;
}

void UCameraModeComponent::UpdateSpringArmPivotLocation(float DeltaTime)
{
	const auto currentLocation = Character->GetCameraBoom()->GetRelativeLocation();

	const auto targetLocation = OriginSpringArmRelativeLocation + CurrentCameraMode->SpringArmPivotLocationOffset;

	const auto newLocation = FMath::VInterpTo(currentLocation, targetLocation, DeltaTime, GetInterpSpeed());

	Character->GetCameraBoom()->SetRelativeLocation(newLocation);
}

void UCameraModeComponent::UpdateCameraLocation(float DeltaTime)
{
	const auto currentLocation = Character->GetCameraBoom()->SocketOffset;
	const auto targetLocation = CurrentCameraMode->CameraOffset;
	FVector newLocation = FMath::VInterpTo(currentLocation, targetLocation, DeltaTime, GetInterpSpeed());

	Character->GetCameraBoom()->SocketOffset = newLocation;
}

void UCameraModeComponent::UpdateFOV(float DeltaTime)
{
	const auto currentFov = PlayerCameraManager->GetFOVAngle();
	const auto targetFov = CurrentCameraMode->Fov;
	auto newFov = FMath::FInterpTo(currentFov, targetFov, DeltaTime, GetInterpSpeed());
	
	PlayerCameraManager->SetFOV(newFov);
}