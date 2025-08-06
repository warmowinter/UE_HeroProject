// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class AIMTOSHOOT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalComponent")
		USkeletalMeshComponent* SkeleMesh;

	virtual void WeaponStartFire();
	virtual void WeaponStopFire();
	virtual void WeaponFiring();
protected:
	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		float FireRate;
	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		float BaseDamage;
	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		float RecoilStrength;
	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		int32 CurrentAmmo;
	UPROPERTY(EditAnywhere, Category = "WeaponStats")
		TSubclassOf<class ABulletsBase> BulletClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
