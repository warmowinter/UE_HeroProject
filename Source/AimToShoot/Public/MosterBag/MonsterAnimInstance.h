// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

class AMosterBase;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation()override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonAnim")
		AMosterBase* CurrentMonster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonAnim")
		UCharacterMovementComponent* MonsterMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonAnim")
		float XY_Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonAnim")
		bool Anim_IsAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonAnim")
		bool IsDead;
};
