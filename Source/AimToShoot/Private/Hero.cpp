// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"
#include "Components/InputComponent.h"
#include "ItemActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "InventoryMangerInstance.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TurnRateBase = 50.0f;
	if (RootComponent == nullptr) {
		RootComponent = GetCapsuleComponent();
	}
	MyMeshComponent = GetMesh();

	//防止角色旋转跟随鼠标
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//启用角色朝移动方向转向,且配置转向速率
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	//set mesh location and rotation设置位置和旋转
	MyMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MyMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//第三人称视角
	SpringRob = CreateDefaultSubobject<USpringArmComponent>(TEXT("CaremaBoom"));
	SpringRob->SetupAttachment(RootComponent);
	SpringRob->TargetArmLength = 400.0f;
	SpringRob->bUsePawnControlRotation = true;

	MyCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	MyCameraComponent->SetupAttachment(SpringRob,USpringArmComponent::SocketName);
	MyCameraComponent->bUsePawnControlRotation = false;

	//仿2.5D视角
	SpringRob_2D = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom2"));
	SpringRob_2D->SetupAttachment(RootComponent);
	SpringRob_2D->TargetArmLength = 1200.0f;
	SpringRob_2D->bUsePawnControlRotation = false;//弹簧臂不跟随控制器旋转而旋转
	SpringRob_2D->SetUsingAbsoluteRotation(true);//设置世界绝对旋转，不跟随角色旋转而旋转

	MyCameraComponent_2D = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp2"));
	MyCameraComponent_2D->SetupAttachment(SpringRob_2D, USpringArmComponent::SocketName);
	MyCameraComponent_2D->bUsePawnControlRotation = false;
	
	//视角初始化设置
	CurrentCamera = MyCameraComponent;//设置当前摄像机
	MyCameraComponent->SetActive(true);//激活第一个摄像机
	MyCameraComponent_2D->SetActive(false);
	Is_2D_View = false;

	//拾取范围初始化
	PickUpRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpRange"));
	PickUpRange->SetupAttachment(RootComponent);
	PickUpRange->SetSphereRadius(200.f);
	PickUpRange->SetCollisionProfileName(TEXT("OverLapAll"));

	//武器插槽
	WeaponSocketMap.Add(1, "Spine_Weapon1");
	WeaponSocketMap.Add(2, "spine_01_left");
	WeaponSocketMap.Add(3, "spine_01_right");
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	//角色血条体力初始化
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaDrainRate = 10.0f;
	//背包初始化
	BackPackArray.SetNum(12);
	//装备数组初始化
	EquipArray.SetNum(6);

	//创建生命和体力值界面
	if (HeroWidgetClass) {
		HeroWidget = CreateWidget<UHero_Widget>(GetWorld(),HeroWidgetClass);
		if (HeroWidget) {
			HeroWidget->AddToViewport();
		}
	}
	HeroWidget->UpDateHealth(this);
	HeroWidget->UpDateStamina(this);
	//创建背包界面
	if (BackPackWidgetClass) {
		BackPackUI = CreateWidget<UBackPackWidget>(GetWorld(), BackPackWidgetClass);
		if (BackPackUI) {
			BackPackUI->AddToViewport();
			BackPackUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//背包管理绑定角色
	UInventoryMangerInstance* BPSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryMangerInstance>();
	BPSubsystem->Bind_Chara = this;
	////配置枪
	//if (ConfigWeapon) {
	//	FActorSpawnParameters SpawnParams;
	//	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon);
	//	CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("hand_r_Weapon"));
	//	CurrentWeapon->SetActorHiddenInGame(false);//暂时不隐藏，后续背包出来再开发
	//	CurrentWeapon->SetOwnerHero(this);
	//}


}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Is_Run && CurrentStamina) {
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		HeroWidget->UpDateStamina(this);
		if (CurrentStamina <= 0) {
			CurrentStamina = 0;
			StopFastRun();
		}
	}
	if (Is_Run == false && CurrentStamina <= MaxStamina) {
		HeroWidget->UpDateStamina(this);
		CurrentStamina += StaminaDrainRate * DeltaTime;
	}

}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHero::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHero::StopJumping);
	//fast run
	PlayerInputComponent->BindAction("FastRun", IE_Pressed, this, &AHero::FastRun);
	PlayerInputComponent->BindAction("FastRun", IE_Released, this, &AHero::StopFastRun);

	//PickUp
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AHero::TryPickUp);
	//Open BackPack
	PlayerInputComponent->BindAction("OpenBP", IE_Pressed, this, &AHero::OpenBackPack);

	//武器相关
	PlayerInputComponent->BindAction("Select 1", IE_Pressed, this, &AHero::EquipWeapon1);
	PlayerInputComponent->BindAction("Select 2", IE_Pressed, this, &AHero::EquipWeapon2);
	PlayerInputComponent->BindAction("Select 3", IE_Pressed, this, &AHero::EquipWeapon3);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AHero::SwitchWeaponAttachment);	//切换武器装备

	PlayerInputComponent->BindAction("CharaFire", IE_Pressed, this, &AHero::StartFire);		//Fire
	PlayerInputComponent->BindAction("CharaFire", IE_Released, this, &AHero::StopFire);

	PlayerInputComponent->BindAction("ChangeBullet", IE_Pressed, this, &AHero::NoticeChangeBullet);	//change Bullet


	//move
	PlayerInputComponent->BindAxis("Move Forward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AHero::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AHero::LookUpRate);

	//toggle view
	PlayerInputComponent->BindAction("ToggleView", IE_Pressed, this, &AHero::ToggleCharacterView);


}

void AHero::AttackMoster(AMosterBase* Target, const FHitResult& Hit)
{
	if (!Target)	return;
	//命中或者啥啥啥-占个位置
	OnMosterAttacked.Broadcast(this, Target, Hit);

}

void AHero::MoveForward(float value) {
	if (Controller != nullptr && value != 0.0f && !Is_2D_View) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
	else {
		FVector Forward = FVector::ForwardVector;
		AddMovementInput(Forward, value);
	}
}

void AHero::MoveRight(float value) {
	if (Controller != nullptr && value != 0.0f && !Is_2D_View) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
	else {
		FVector Right = FVector::RightVector;
		AddMovementInput(Right, value);
	}
}

void AHero::TurnAtRate(float rate) {
	AddControllerYawInput(rate * TurnRateBase * GetWorld()->GetDeltaSeconds());
}

void AHero::LookUpRate(float rate) {
	AddControllerPitchInput(rate * TurnRateBase * GetWorld()->GetDeltaSeconds());
}


void AHero::ToggleCharacterView() {
	if (!Is_2D_View) {
		CurrentCamera = MyCameraComponent_2D;
		MyCameraComponent->SetActive(false);
		MyCameraComponent_2D->SetActive(true);
		Is_2D_View = true;
	}
	else {
		CurrentCamera = MyCameraComponent;
		MyCameraComponent_2D->SetActive(false);
		MyCameraComponent->SetActive(true);
		Is_2D_View = false;
	}


	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController && CurrentCamera) {
		PlayerController->SetViewTargetWithBlend(this, 1.0f);
	}
}



float AHero::GetCharaMaxHealth() {
	return this->MaxHealth;
}

float AHero::GetCharaCurrentHealth() {
	return this->CurrentHealth;
}

void AHero::CurrentTakeDamage(float DamageAmount) {
	CurrentHealth -= DamageAmount;
	HeroWidget->UpDateHealth(this);

	if (CurrentHealth <= 0) {
		
	}
}

void AHero::FastRun() {
	if (CurrentStamina) {
		Is_Run = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AHero::StopFastRun() {
	Is_Run = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------
角色背包模块-主要：通过角色的物品数组进行一系列操作
背包结构：可查询AllDateDefine.h		拾取物品：TryPickUp()  --在检测到武器时自动创建Weapon类		添加物品：AddItemToBackPack(const FBackPackStruct& NewItem)
打开背包/关闭背包：OpenBackPack()

--------------------------------------------------------------------------------------------------------------------------------------------*/
//我这个添加物品函数，是建立在数组已有背包结构全部初始化的信息，根据背包结构的信息来进行修改，达到物品添加的效果
void AHero::AddItemToBackPack(const FBackPackStruct& NewItem) {
	int32 EmptySlotIndex = -1;

	if (NewItem.bIsStackable) {
		// 先找可堆叠物品
		for (int32 i = 0; i < BackPackArray.Num(); i++) {
			if (BackPackArray[i].ItemID == NewItem.ItemID && BackPackArray[i].Quantity < BackPackArray[i].MaxStackNumber) {
				BackPackArray[i].Quantity += NewItem.Quantity;
				return;
			}
			if (BackPackArray[i].ItemID == 0 && EmptySlotIndex == -1) {
				EmptySlotIndex = i;
			}
		}
	}
	else {
		// 不可堆叠物品直接找空位
		for (int32 i = 0; i < BackPackArray.Num(); i++) {
			if (BackPackArray[i].ItemID == 0) {
				BackPackArray[i] = NewItem;
				return;
			}
		}
	}

	// 如果可堆叠但没找到相同物品，就用空位
	if (EmptySlotIndex != -1) {
		BackPackArray[EmptySlotIndex] = NewItem;
	}
}
//拾取函数
void AHero::TryPickUp() {
	TArray<AActor*> OverlappingActors;
	PickUpRange->GetOverlappingActors(OverlappingActors, AItemActor::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("TryPickUp Callable"));
	for (AActor* Actor : OverlappingActors) {
		if (AItemActor* Item = Cast<AItemActor>(Actor)) {
			FVector SpawnLocation(0.0f, 0.0f, -999.0f);
			FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			if (Item->ItemInfo.WeaponNumber == 1) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon1,SpawnLocation,SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			else if (Item->ItemInfo.WeaponNumber == 2) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon2, SpawnLocation, SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			else if (Item->ItemInfo.WeaponNumber == 3) {
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon3, SpawnLocation, SpawnRotation);
				Item->ItemInfo.Actor_Ptr = wuhu;
			}
			AddItemToBackPack(Item->ItemInfo);
			Item->Destroy();
			break;
		}
	}
	NoticeRefresh();
}

void AHero::NoticeRefresh() {
	if (BackPackUI) {
		BackPackUI->RefreshBackPack(BackPackArray, EquipArray);
	}
}

void AHero::OpenBackPack() {
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!Is_OpenBP) {
		if (!BackPackUI)	return;
		BackPackUI->SetVisibility(ESlateVisibility::Visible);
		NoticeRefresh();
		UE_LOG(LogTemp, Log, TEXT("open BackPack"));
		Is_OpenBP = true;

		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(BackPackUI->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
	else {
		BackPackUI->SetVisibility(ESlateVisibility::Hidden);
		Is_OpenBP = false;
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

}
//类外获取当前背包数组
TArray<FBackPackStruct>& AHero::GetBackPackArray() {
	return BackPackArray;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------
角色武器模块-主要：通过角色的装备数组进行一系列操作
选择武器：GetWeaponFromEquipGrid(int32 Index)		武器挂接点初始化：InitializeEquipments()		武器移除：RemoveWeapon()
快速切枪：SwitchWeaponAttachment()					武器换弹通知NoticeChangeBullet()				武器开火/停火：StartFire() / StopFire()
武器射击动画播放：PlayFireMontage(UAnimMontage* Montage)
--------------------------------------------------------------------------------------------------------------------------------------------*/


void AHero::StartFire() {
	if (CurrentWeapon && CurrentWeapon->GetAttachParentSocketName() == TEXT("hand_r_Weapon")) {
		Is_FireState = true;
		CurrentWeapon->WeaponStartFire();
	}
}
//这个函数可能有隐患，当角色在射击过程中，切换武器了，那么停止开火这个就不会调用吧
void AHero::StopFire() {
	if (CurrentWeapon) {
		Is_FireState = false;
		CurrentWeapon->WeaponStopFire();
	}
}

//选择武器瞄准,仅实验三把
void AHero::GetWeaponFromEquipGrid(int32 Index) {
	if (Index < 0 || Index >= 3)	return;
	StopFire();//直接在切换时调用
	AWeaponBase* NewWeapon = EquipArray[Index].Actor_Ptr;
	if (!NewWeapon)	return;

	if (CurrentWeapon) {
		FName BackSocket = "";
		for (int32 i = 0; i < 3; i++) {
			if (CurrentWeapon == EquipArray[i].Actor_Ptr) {
				BackSocket = WeaponSocketMap[i+1];
			}
		}
		CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, BackSocket);

	}

	CurrentWeapon = NewWeapon;
	CurrentWeapon->SetOwnerHero(this);
	CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_r_Weapon");
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	Is_AimState = true;
}


void AHero::InitializeEquipments() {
	for (int32 i = 0; i < 3; i++) {
		if (EquipArray[i].Actor_Ptr != nullptr) {
			FName SockerName;
			switch (i)
			{
			case 0:SockerName = "Spine_Weapon1"; break;//背上
			case 1:SockerName = "spine_01_left"; break;//左屁股
			case 2:SockerName = "spine_01_right"; break;//右屁股
			default:SockerName = "calf_r_Weapon"; break;//默认右腿咯，先只搞三把武器,这个就不先延伸
			}
			EquipArray[i].Actor_Ptr->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SockerName);

		}
	}
}

void AHero::EquipWeapon1() { 
	UE_LOG(LogTemp, Log, TEXT("input select 1"));
	GetWeaponFromEquipGrid(0); 
}
void AHero::EquipWeapon2() { 
	UE_LOG(LogTemp, Log, TEXT("input select 2"));
	GetWeaponFromEquipGrid(1); 
}
void AHero::EquipWeapon3() { 
	UE_LOG(LogTemp, Log, TEXT("input select 3"));
	GetWeaponFromEquipGrid(2); 
}

//装备武器，仅展示前两把武器，后期拓展再改
//暂时不用，维护修改
void AHero::EquipWeapon() {
	//if (EquipArray[0].ItemType == EItemType::EIT_Weapon && EquipArray[0].WeaponNumber == 1) {
	//	CurrentWeapon = EquipArray[0].Actor_Ptr;
	//	CurrentWeapon->SetOwnerHero(this);
	//	CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Spine_Weapon1"));
	//}

}
void AHero::RemoveWeapon() {
	for (int32 i = 0; i < 3; i++) {
		if (EquipArray[i].Actor_Ptr == nullptr) {

			AWeaponBase* WeaponOnBack = nullptr;

			FName SocketName;
			switch (i)
			{
			case 0: SocketName = "Spine_Weapon1"; break;
			case 1: SocketName = "spine_01_left"; break;
			case 2: SocketName = "spine_01_right"; break;
			default: SocketName = "calf_r_Weapon"; break;
			}
			TArray<AActor*>	AttachedWeapons;
			GetAttachedActors(AttachedWeapons);
			for (AActor* Actor : AttachedWeapons) {
				AWeaponBase* Weapon = Cast<AWeaponBase>(Actor);
				if (Weapon && (Weapon->GetAttachParentSocketName() == SocketName || Weapon->GetAttachParentSocketName() == "hand_r_Weapon")) {
					WeaponOnBack = Weapon;
					break;
				}
			}

			if (WeaponOnBack) {
				if (WeaponOnBack == CurrentWeapon) {
					CurrentWeapon = nullptr;
				}

				WeaponOnBack->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				FVector SpawnLocation(0.0f, 0.0f, -999.0f);
				FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
				WeaponOnBack->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
				bUseControllerRotationYaw = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
				Is_AimState = false;
			}


			//if (CurrentWeapon == EquipArray[i].Actor_Ptr) {
			//	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			//	FVector SpawnLocation(0.0f, 0.0f, -999.0f);
			//	FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			//	CurrentWeapon->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
			//	CurrentWeapon = nullptr;
			//	bUseControllerRotationYaw = false;
			//	GetCharacterMovement()->bOrientRotationToMovement = true;
			//	Is_AimState = false;
			//}
		}
	}
}
//快速切枪，这个后面再改，因为现在没做其他武器的手持挂接点
void AHero::SwitchWeaponAttachment() {
	if (CurrentWeapon) {
		if (CurrentWeapon->GetAttachParentSocketName() == TEXT("Spine_Weapon1")) {
			CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r_Weapon"));
			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;
			Is_AimState = true;
		}
		else {
			CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Spine_Weapon1"));
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;
			Is_AimState = false;
		}
	}
}

void AHero::NoticeChangeBullet() {
	if (!Is_ChangeBullet && CurrentWeapon) {
		CurrentWeapon->ChangeBullet();
		Is_ChangeBullet = true;
	}
}

void AHero::PlayFireMontage(UAnimMontage* Montage) {
	if (Montage && GetMesh()) {
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		float Duration = GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		UE_LOG(LogTemp, Log, TEXT("Montage_Play called, Duration= %f"), Duration);
	}
}

bool AHero::GetCharaState() {
	return Is_AimState;
}

bool AHero::GetFireState() {
	return Is_FireState;
}
