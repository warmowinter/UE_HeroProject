// Fill out your copyright notice in the Description page of Project Settings.


#include "MosterBag/MonsterAnimInstance.h"
#include "MosterBag/MosterBase.h"
#include "GameFramework/CharacterMovementComponent.h"



void UMonsterAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	
	XY_Speed = 0;
	Anim_IsAttack = false;
	IsDead = false;
	CurrentMonster = nullptr;
	MonsterMovementComp = nullptr;

	CurrentMonster = Cast<AMosterBase>(TryGetPawnOwner());
	if (CurrentMonster) {
		MonsterMovementComp = CurrentMonster->GetCharacterMovement();
	}
}



void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CurrentMonster || !MonsterMovementComp)	return;

	FVector Mon_Velocity = CurrentMonster->GetVelocity();
	XY_Speed = Mon_Velocity.Size2D();

	Anim_IsAttack = CurrentMonster->GetMonsterAttackState();


}
