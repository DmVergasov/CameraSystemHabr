// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CameraModeComponent.generated.h"

class UCameraMode;
class ACameraSystemCharacter;
class APlayerCameraManager;

USTRUCT(BlueprintType)
struct FCameraModeData
{
	GENERATED_BODY()
public:
	bool CanActivateMode(const FGameplayTagContainer& TagsToCheck) const
	{
		return ModeActivationConditions.IsEmpty() || ModeActivationConditions.Matches(TagsToCheck);
	}

	UCameraMode* GetCameraMode() const { return CameraMode; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UCameraMode* CameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagQuery ModeActivationConditions;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMERASYSTEMHABR_API UCameraModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraModeComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnAbilityTagChanged(const FGameplayTag& Tag, bool TagExists);

	void TryUpdateCameraMode();

	void SetCameraMode(UCameraMode* NewMode);
	UCameraMode* DetermineCameraMode(const FGameplayTagContainer& Tags) const;

	float GetInterpSpeed() const;
	void UpdateCameraMode(float DeltaTime);
	void UpdateSpringArmLength(float DeltaTime);
	void UpdateSpringArmPivotLocation(float DeltaTime);
	void UpdateCameraLocation(float DeltaTime);
	void UpdateFOV(float DeltaTime);

	APlayerCameraManager* GetPlayerCameraManager() const;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FCameraModeData> CameraModes;

	UPROPERTY(BlueprintReadOnly)
	UCameraMode* CurrentCameraMode;

	TWeakObjectPtr <ACameraSystemCharacter> Character;

	FVector OriginSpringArmRelativeLocation;

	float TimeSecondsAfterSetNewMode = 0.f;

	float PreviousInterpSpeed = 0.f;
};
