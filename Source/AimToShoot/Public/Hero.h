// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero_Widget.h"
#include "BackPackWidget.h"
#include "WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "DataDefine/AllDataDefine.h"
#include "MosterBag/MosterBase.h"
#include "Hero.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMosterAttacked, ACharacter*, Attacker, AMosterBase*, Victim);

UCLASS()
class AIMTOSHOOT_API AHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHero();
	//角色组件属性：角色网格体可编辑，暴露在蓝图，弹簧臂和摄像机
	UPROPERTY()
		USkeletalMeshComponent* MyMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringRob;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* MyCameraComponent;
	//角色2.5D视角：弹簧臂建立和摄像机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera2.5D")
		class USpringArmComponent* SpringRob_2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera2.5D")
		UCameraComponent* MyCameraComponent_2D;
	//UI血条体力界面
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> HeroWidgetClass;
	//UI血条体力实例化
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		UHero_Widget* HeroWidget;
	//UI背包界面
	UPROPERTY(EditAnywhere, Category = "BackPackUI")
		TSubclassOf<UUserWidget> BackPackWidgetClass;
	//UI背包界面实例化
	UPROPERTY(BlueprintReadWrite, Category = "BackPackUI")
		UBackPackWidget* BackPackUI;


	//拾取范围组件
	UPROPERTY(VisibleAnywhere)
		USphereComponent* PickUpRange;
	//体力属性
	float StaminaDrainRate;
	//配备武器类型
	UPROPERTY(EditAnywhere, Category = "WeaponClass")
		TSubclassOf<AWeaponBase> ConfigWeapon;
	//当前武器实例化
	AWeaponBase* CurrentWeapon = nullptr;

public:

	//多播事件-怪物Monster
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnMosterAttacked OnMosterAttacked;

	UFUNCTION(BlueprintCallable)
		void AttackMoster(AMosterBase* Target);

protected:
	//背包数组
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Inventory")
		TArray<FBackPackStruct> BackPackArray;
	//装备格数组
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip_State")
		TArray<FBackPackStruct>	EquipArray;

public:
	//默认视角转速，
	UPROPERTY(VisibleAnywhere)
		float TurnRateBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentCamera")
		UCameraComponent* CurrentCamera;

protected:
	//角色移动，视角输入
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float rate);
	void LookUpRate(float rate);


protected:
	//角色状态State
	UPROPERTY(EditAnywhere, Category = "State")
		float MaxHealth;//健康值
	UPROPERTY(EditAnywhere, Category = "State")
		float CurrentHealth;//当前健康值

	UPROPERTY(EditAnywhere, Category = "State")
		float MaxStamina;//体力值
	UPROPERTY(EditAnywhere, Category = "State")
		float CurrentStamina;//当前体力值



	bool Is_Run = false;//是否在快跑
	bool Is_OpenBP = false;//是否打开了背包
	bool Is_ChangeBullet = false; //是否在换弹
public:
	//切换视图
	UFUNCTION(BlueprintCallable, Category = "ToggleCharacterView")
		void ToggleCharacterView();
	UPROPERTY()
		bool Is_2D_View;

	//调用获取生命值和体力值
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
		float GetCharaMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
		float GetCharaCurrentHealth();
	friend void UHero_Widget::UpDateHealth(AHero* Object);
	friend void UHero_Widget::UpDateStamina(AHero* Object);

	//奔跑
		void FastRun();
		void StopFastRun();
	//受击函数
	void CurrentTakeDamage(float DamageAmount);
	//开火函数,
	void StartFire();
	void StopFire();
	//拾取东西函数
	void TryPickUp();
	//添加物品到背包中函数
	void AddItemToBackPack(const FBackPackStruct& NewItem);
	//通知背包刷新的函数
	void NoticeRefresh();
	//打开背包函数
	void OpenBackPack();
	//外界获取背包数组
	TArray<FBackPackStruct>& GetBackPackArray();

	//装备武器
	void EquipWeapon();
	//卸下武器
	void RemoveWeapon();
	//切换武器附着点
	void SwitchWeaponAttachment();

	//换弹通知
	void NoticeChangeBullet();
	friend void AWeaponBase::ChangeBullet();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
