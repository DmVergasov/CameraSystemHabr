// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "CameraSystemCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS(config=Game)
class ACameraSystemCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACameraSystemCharacter();

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void AddTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetGameplayTags() const;

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagContainerChanged, const FGameplayTag&, ChangedTag, bool, bExist);

	UPROPERTY(BlueprintAssignable)
	FOnTagContainerChanged OnTagContainerChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	TMap<FGameplayTag, int32> TagMap;
};

