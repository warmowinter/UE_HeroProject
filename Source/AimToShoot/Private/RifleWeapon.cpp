// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"

ARifleWeapon::ARifleWeapon() {
	FireRate = 0.1f;
}


void ARifleWeapon::WeaponFiring() {
	UE_LOG(LogTemp, Warning, TEXT("Rifle Fire"));
	if (CurrentAmmo <= 0) {
		UE_LOG(LogTemp, Log, TEXT("No Ammo"));
		return;
	}
	CurrentAmmo--;
	if (BulletClass) {
		FVector MuzzleLocation = SkeleMesh->GetSocketLocation("FIreMuzzle");
		FRotator MuzzleRotation = SkeleMesh->GetSocketRotation("FIreMuzzle");

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABulletsBase* Bullet_A = GetWorld()->SpawnActor<ABulletsBase>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Bullet_A) {
			FVector LaunchDirection = MuzzleRotation.Vector();
			Bullet_A->FireInDirection(LaunchDirection);
		}

	}
}