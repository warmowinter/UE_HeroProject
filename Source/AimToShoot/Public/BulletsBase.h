// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletsBase.generated.h"

UCLASS()
class AIMTOSHOOT_API ABulletsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletsBase();

	UPROPERTY(VisibleAnywhere, Category = "CollComponent")
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "BullComponent")
		class UStaticMeshComponent* BullMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Movement_A")
		class UProjectileMovementComponent* ProjectileMovement;
public:
	UFUNCTION(BlueprintCallable, Category = "OnHit")
		void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "FireDirection")
		void FireInDirection(const FVector& ShootDirection);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
