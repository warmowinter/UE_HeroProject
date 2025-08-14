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
	//��ɫ������ԣ���ɫ������ɱ༭����¶����ͼ�����ɱۺ������
	UPROPERTY()
		USkeletalMeshComponent* MyMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringRob;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* MyCameraComponent;
	//��ɫ2.5D�ӽǣ����ɱ۽����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera2.5D")
		class USpringArmComponent* SpringRob_2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera2.5D")
		UCameraComponent* MyCameraComponent_2D;
	//UIѪ����������
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> HeroWidgetClass;
	//UIѪ������ʵ����
	UPROPERTY(BlueprintReadWrite, Category = "UI")
		UHero_Widget* HeroWidget;
	//UI��������
	UPROPERTY(EditAnywhere, Category = "BackPackUI")
		TSubclassOf<UUserWidget> BackPackWidgetClass;
	//UI��������ʵ����
	UPROPERTY(BlueprintReadWrite, Category = "BackPackUI")
		UBackPackWidget* BackPackUI;


	//ʰȡ��Χ���
	UPROPERTY(VisibleAnywhere)
		USphereComponent* PickUpRange;
	//��������
	float StaminaDrainRate;
	//�䱸��������
	UPROPERTY(EditAnywhere, Category = "WeaponClass")
		TSubclassOf<AWeaponBase> ConfigWeapon;
	//��ǰ����ʵ����
	AWeaponBase* CurrentWeapon = nullptr;

public:

	//�ಥ�¼�-����Monster
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnMosterAttacked OnMosterAttacked;

	UFUNCTION(BlueprintCallable)
		void AttackMoster(AMosterBase* Target);

protected:
	//��������
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Inventory")
		TArray<FBackPackStruct> BackPackArray;
	//װ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip_State")
		TArray<FBackPackStruct>	EquipArray;

public:
	//Ĭ���ӽ�ת�٣�
	UPROPERTY(VisibleAnywhere)
		float TurnRateBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentCamera")
		UCameraComponent* CurrentCamera;

protected:
	//��ɫ�ƶ����ӽ�����
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float rate);
	void LookUpRate(float rate);


protected:
	//��ɫ״̬State
	UPROPERTY(EditAnywhere, Category = "State")
		float MaxHealth;//����ֵ
	UPROPERTY(EditAnywhere, Category = "State")
		float CurrentHealth;//��ǰ����ֵ

	UPROPERTY(EditAnywhere, Category = "State")
		float MaxStamina;//����ֵ
	UPROPERTY(EditAnywhere, Category = "State")
		float CurrentStamina;//��ǰ����ֵ



	bool Is_Run = false;//�Ƿ��ڿ���
	bool Is_OpenBP = false;//�Ƿ���˱���
	bool Is_ChangeBullet = false; //�Ƿ��ڻ���
public:
	//�л���ͼ
	UFUNCTION(BlueprintCallable, Category = "ToggleCharacterView")
		void ToggleCharacterView();
	UPROPERTY()
		bool Is_2D_View;

	//���û�ȡ����ֵ������ֵ
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
		float GetCharaMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
		float GetCharaCurrentHealth();
	friend void UHero_Widget::UpDateHealth(AHero* Object);
	friend void UHero_Widget::UpDateStamina(AHero* Object);

	//����
		void FastRun();
		void StopFastRun();
	//�ܻ�����
	void CurrentTakeDamage(float DamageAmount);
	//������,
	void StartFire();
	void StopFire();
	//ʰȡ��������
	void TryPickUp();
	//�����Ʒ�������к���
	void AddItemToBackPack(const FBackPackStruct& NewItem);
	//֪ͨ����ˢ�µĺ���
	void NoticeRefresh();
	//�򿪱�������
	void OpenBackPack();
	//����ȡ��������
	TArray<FBackPackStruct>& GetBackPackArray();

	//װ������
	void EquipWeapon();
	//ж������
	void RemoveWeapon();
	//�л��������ŵ�
	void SwitchWeaponAttachment();

	//����֪ͨ
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
