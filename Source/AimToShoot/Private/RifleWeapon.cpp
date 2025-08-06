// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"

ARifleWeapon::ARifleWeapon() {
	FireRate = 4.0f;
}


void ARifleWeapon::WeaponFiring() {
	UE_LOG(LogTemp, Warning, TEXT("Rifle Fire"));
}