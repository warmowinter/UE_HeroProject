// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BulletsBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponBase.generated.h"


class AHero;

UCLASS()
class AIMTOSHOOT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalComponent")
		USkeletalMeshComponent* SkeleMesh;
	//开火开始时，开火禁止时，开火进行时
	void WeaponStartFire();
	void WeaponStopFire();
	virtual void WeaponFiring();
	//换弹，设置该武器的拥有者
	void ChangeBullet();
	void SetOwnerHero(AHero* InOwnerHero);
public:
	AHero* OwnerHero = nullptr;


protected:
	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		float FireRate;
	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		float BaseDamage;
	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		float RecoilStrength;
	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		int32 CurrentAmmo;
	UPROPERTY(EditAnywhere, Category = "WeaponProperty")
		TSubclassOf<ABulletsBase> BulletClass;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
