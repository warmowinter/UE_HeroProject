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

	//

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
	EquipArray.SetNum(22);

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
	//Fire
	PlayerInputComponent->BindAction("CharaFire",IE_Pressed,this,&AHero::StartFire);
	PlayerInputComponent->BindAction("CharaFire",IE_Released,this,&AHero::StopFire);
	//change Bullet
	PlayerInputComponent->BindAction("ChangeBullet", IE_Pressed, this, &AHero::NoticeChangeBullet);
	//PickUp
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AHero::TryPickUp);
	//Open BackPack
	PlayerInputComponent->BindAction("OpenBP", IE_Pressed, this, &AHero::OpenBackPack);
	//切换武器装备
	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AHero::SwitchWeaponAttachment);

	//move
	PlayerInputComponent->BindAxis("Move Forward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AHero::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AHero::LookUpRate);

	//toggle view
	PlayerInputComponent->BindAction("ToggleView", IE_Pressed, this, &AHero::ToggleCharacterView);


}

void AHero::AttackMoster(AMosterBase* Target)
{
	if (!Target)	return;
	//命中或者啥啥啥-占个位置
	OnMosterAttacked.Broadcast(this, Target);

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
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

void AHero::StopFastRun() {
	Is_Run = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AHero::StartFire() {
	if (CurrentWeapon && CurrentWeapon->GetAttachParentSocketName() == TEXT("hand_r_Weapon")) {
		CurrentWeapon->WeaponStartFire();
	}
}
void AHero::StopFire() {
	if (CurrentWeapon) {
		CurrentWeapon->WeaponStopFire();
	}
}
//我这个添加物品函数，是建立在数组已有背包结构全部初始化的信息，根据背包结构的信息来进行修改，达到物品添加的效果
void AHero::AddItemToBackPack(const FBackPackStruct& NewItem) {
	int32 EmptySlotIndex = -1;

	if (NewItem.bIsStackable) {
		// 先找可堆叠物品
		for (int32 i = 0; i < BackPackArray.Num(); i++) {
			if (BackPackArray[i].ItemID == NewItem.ItemID) {
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
			if (Item->ItemInfo.WeaponNumber == 1) {
				FVector SpawnLocation(0.0f, 0.0f, -999.0f);
				FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
				AWeaponBase* wuhu = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon,SpawnLocation,SpawnRotation);
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

TArray<FBackPackStruct>& AHero::GetBackPackArray() {
	return BackPackArray;
}



void AHero::EquipWeapon() {
	for (int32 i = 0; i < EquipArray.Num(); i++) {
		if (EquipArray[i].ItemType == EItemType::EIT_Weapon && EquipArray[i].WeaponNumber == 1) {

			CurrentWeapon = EquipArray[i].Actor_Ptr;
			CurrentWeapon->SetOwnerHero(this);
			CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Spine_Weapon1"));
		}

	}
}
void AHero::RemoveWeapon() {
	for (int32 i = 0; i < EquipArray.Num(); i++) {
		if (EquipArray[i].ItemType != EItemType::EIT_Weapon) {
			if (CurrentWeapon) {
				CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				FVector SpawnLocation(0.0f, 0.0f, -999.0f);
				FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
				CurrentWeapon->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
				CurrentWeapon = nullptr;
			}
		}
	}
}

void AHero::SwitchWeaponAttachment() {
	if (CurrentWeapon) {
		if (CurrentWeapon->GetAttachParentSocketName() == TEXT("Spine_Weapon1")) {
			CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r_Weapon"));
		}
		else {
			CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Spine_Weapon1"));
		}
	}
}

void AHero::NoticeChangeBullet() {
	if (!Is_ChangeBullet && CurrentWeapon) {
		CurrentWeapon->ChangeBullet();
		Is_ChangeBullet = true;
	}
}