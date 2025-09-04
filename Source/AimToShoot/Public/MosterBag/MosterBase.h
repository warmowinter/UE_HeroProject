// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MosterBase.generated.h"


class AHero;
class USphereComponent;

UCLASS()
class AIMTOSHOOT_API AMosterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMosterBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		USkeletalMeshComponent* MonsterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereComp")
		USphereComponent* SphereComp;

public:
	//处理伤害通知
	UFUNCTION()
		void HandleAttacked(class AHero* Attacker, AMosterBase* Victim, const FHitResult& Hit);
	//死亡通知
	UFUNCTION()
		void MosterDie();
	UFUNCTION()
		void OnDeathAnimationFinished();


	//被命中拉仇恨，类似我的世界猪人效果
	UFUNCTION(BlueprintCallable)
		void ReceiveHitFrom(ACharacter* Attacker);
	UFUNCTION(BlueprintCallable)
		void OnNearbyCombat(ACharacter* Attacker, AMosterBase* Victim);


	void DoSphereTrace();
public:
	//类外获取函数集合
	UFUNCTION()
		bool GetMonsterAttackState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		float MaxWalk;//呃后续可以延伸，怪物看到人加速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		float AttackBase;

	//角色指针
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AHero* Player;

	//怪物状态
	bool IsDead;//怪物是否死亡
	bool EnemyIsNearby;//敌人是否在附近
	bool IsAttack;//怪物是否攻击


protected:
	//怪物Montage相关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterMontage")
		UAnimMontage* DeathMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)	override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
