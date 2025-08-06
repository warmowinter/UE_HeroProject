// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SkeleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));


	FireRate = 1.0f;
	BaseDamage = 1.0f;
	RecoilStrength = 1.0f;
	MaxAmmo = 7;
	CurrentAmmo = 0;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
	}
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::WeaponStartFire() {
	WeaponFiring();
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AWeaponBase::WeaponFiring, FireRate, true);
	}

}

void AWeaponBase::WeaponStopFire() {
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
	}
}

void AWeaponBase::WeaponFiring() {
	UE_LOG(LogTemp, Log, TEXT("WeaponBase Firing"));
}