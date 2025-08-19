// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Hero.h"
#include "MosterBag/MosterBase.h"
#include "DataDefine/AllDataDefine.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SkeleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComp"));


	FireRate = 1.0f;
	BaseDamage = 1.0f;
	RecoilStrength = 1.0f;
	MaxAmmo = 30;
	CurrentAmmo = 30;
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
		Bullet_A->Bind_Weapon = this;
		if (Bullet_A) {
			FVector LaunchDirection = MuzzleRotation.Vector();
			Bullet_A->FireInDirection(LaunchDirection);
		}

	}
}

void AWeaponBase::SetOwnerHero(AHero* InOwnerHero) {
	OwnerHero = InOwnerHero;
}

void AWeaponBase::NoticeAHeroAttackInfo(AMosterBase* hurted, const FHitResult& Hit)
{
	OwnerHero->AttackMoster(hurted, Hit);
}

void AWeaponBase::ChangeBullet() {
	if (!OwnerHero)	return;
	TArray<FBackPackStruct>& BackPack_g = OwnerHero->GetBackPackArray();
	if (CurrentAmmo == MaxAmmo || OwnerHero && BackPack_g.Num() == 0)	return;
	for (int32 i = 0; i < BackPack_g.Num(); i++) {
		FBackPackStruct& BP_Pro = BackPack_g[i];
		if (BP_Pro.ItemType != EItemType::EIT_Armor)	continue;
		int32 NeedAmmo = MaxAmmo - CurrentAmmo;
		if (BP_Pro.Quantity > NeedAmmo) {
			BP_Pro.Quantity -= NeedAmmo;
			CurrentAmmo = MaxAmmo;
			OwnerHero->NoticeRefresh();
		}
		else {
			CurrentAmmo += BP_Pro.Quantity;
			BP_Pro.Quantity = 0;
		}
		if (BP_Pro.Quantity == 0) {
			BackPack_g[i] = FBackPackStruct();
			OwnerHero->NoticeRefresh();
		}
		OwnerHero->Is_ChangeBullet = false;
		return;
	}
}