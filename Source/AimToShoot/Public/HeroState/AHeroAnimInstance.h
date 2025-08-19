// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AHeroAnimInstance.generated.h"


class AHero;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UAHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	/*初始化调用和每帧更新*/
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		AHero* Hero;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		UCharacterMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		float X_Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		float Y_Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		bool IsEquipWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
		bool IsFireState;
};
