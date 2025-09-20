// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroState/AHeroAnimInstance.h"
#include "Hero.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAHeroAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	Hero = nullptr;
	MovementComp = nullptr;
	X_Speed = 0.0f;
	Y_Speed = 0.0f;
	PitchAngle = 0.f;
	YawAngle = 0.f;

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

	//获取速度
	FVector Velocity = Hero->GetVelocity();
	Velocity.Z = 0;

	//局部速度2D
	FVector LocationVelocity = Hero->GetActorTransform().InverseTransformVector(Velocity);
	X_Speed = LocationVelocity.X;
	Y_Speed = LocationVelocity.Y;

	//获取角色状态
	bIsInAir = MovementComp->IsFalling();
	IsEquipWeapon = Hero->GetCharaState();
	IsFireState = Hero->GetFireState();
	Is_Crouch = Hero->GetHeroIsCrouch();

	//获取AO旋转
	CalculateRotator();
}

void UAHeroAnimInstance::CalculateRotator() {
	//获取Pawn和Acotr的世界旋转
	FRotator Pawn_A = Hero->GetControlRotation();
	FRotator Actor_A = Hero->GetActorRotation();

	
	FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(Pawn_A, Actor_A);

	//限制角度
	RotationDelta.Pitch = FMath::Clamp(RotationDelta.Pitch, -90.f, 90.f);
	/*RotationDelta.Yaw = FMath::Clamp(RotationDelta.Yaw, -180.f, 180.f); *///不需要，默认-180到180
	RotationDelta.Roll = 0.f;

	FRotator CurrentRotation(PitchAngle, YawAngle, 0.f);


	FRotator SmoothRot = FMath::RInterpTo(
		CurrentRotation,
		RotationDelta,
		GetWorld()->GetDeltaSeconds(),
		15.f
	);

	PitchAngle = SmoothRot.Pitch;
	if (abs(RotationDelta.Yaw) >= 175) {
		float SmoothYaw = FMath::FInterpTo(
			CurrentRotation.Yaw,
			0.f,
			GetWorld()->GetDeltaSeconds(),
			15.f
		);
		YawAngle = SmoothYaw;
	}
	else {
		YawAngle = SmoothRot.Yaw;
	}
}