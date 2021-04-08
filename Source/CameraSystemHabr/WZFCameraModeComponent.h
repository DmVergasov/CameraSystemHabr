// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GameplayTagContainer.h>
#include "WZFCameraModeComponent.generated.h"

class UWZFCameraMode;


USTRUCT(BlueprintType)
struct FCameraModeData
{
	GENERATED_BODY()
public:
	bool CanActivateMode(const FGameplayTagContainer& TagsToCheck) const
	{
		return ModeActivationConditions.IsEmpty() || ModeActivationConditions.Matches(TagsToCheck);
	}

	UWZFCameraMode* GetCameraMode() const { return CameraMode; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UWZFCameraMode* CameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagQuery ModeActivationConditions;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMERASYSTEMHABR_API UWZFCameraModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWZFCameraModeComponent();

	UPROPERTY(EditAnywhere)
	TArray<FCameraModeData> CameraModes;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
