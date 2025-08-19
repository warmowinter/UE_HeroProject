// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroState/AHeroAnimInstance.h"
#include "Hero.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAHeroAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	Hero = nullptr;
	MovementComp = nullptr;
	X_Speed = 0.0f;
	Y_Speed = 0.0f;
	bIsInAir = false;
	IsEquipWeapon = false;
	IsFireState = false;
	Hero = Cast<AHero>(TryGetPawnOwner());
	if (Hero) {
		MovementComp = Hero->GetCharacterMovement();
	}
}

void UAHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Hero || !MovementComp) return;

	bIsInAir = MovementComp->IsFalling();

	FVector Velocity = Hero->GetVelocity();
	Velocity.Z = 0;
	FVector LocationVelocity = Hero->GetActorTransform().InverseTransformVector(Velocity);
	X_Speed = LocationVelocity.X;
	Y_Speed = LocationVelocity.Y;
	IsEquipWeapon = Hero->GetCharaState();
	IsFireState = Hero->GetFireState();

}