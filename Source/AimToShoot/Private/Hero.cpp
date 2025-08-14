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

	//��ֹ��ɫ��ת�������
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//���ý�ɫ���ƶ�����ת��,������ת������
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	//set mesh location and rotation����λ�ú���ת
	MyMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MyMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//�����˳��ӽ�
	SpringRob = CreateDefaultSubobject<USpringArmComponent>(TEXT("CaremaBoom"));
	SpringRob->SetupAttachment(RootComponent);
	SpringRob->TargetArmLength = 400.0f;
	SpringRob->bUsePawnControlRotation = true;

	MyCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	MyCameraComponent->SetupAttachment(SpringRob,USpringArmComponent::SocketName);
	MyCameraComponent->bUsePawnControlRotation = false;

	//��2.5D�ӽ�
	SpringRob_2D = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom2"));
	SpringRob_2D->SetupAttachment(RootComponent);
	SpringRob_2D->TargetArmLength = 1200.0f;
	SpringRob_2D->bUsePawnControlRotation = false;//���ɱ۲������������ת����ת
	SpringRob_2D->SetUsingAbsoluteRotation(true);//�������������ת���������ɫ��ת����ת

	MyCameraComponent_2D = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp2"));
	MyCameraComponent_2D->SetupAttachment(SpringRob_2D, USpringArmComponent::SocketName);
	MyCameraComponent_2D->bUsePawnControlRotation = false;
	
	//�ӽǳ�ʼ������
	CurrentCamera = MyCameraComponent;//���õ�ǰ�����
	MyCameraComponent->SetActive(true);//�����һ�������
	MyCameraComponent_2D->SetActive(false);
	Is_2D_View = false;

	//ʰȡ��Χ��ʼ��
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

	//��ɫѪ��������ʼ��
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaDrainRate = 10.0f;
	//������ʼ��
	BackPackArray.SetNum(12);
	//װ�������ʼ��
	EquipArray.SetNum(22);

	//��������������ֵ����
	if (HeroWidgetClass) {
		HeroWidget = CreateWidget<UHero_Widget>(GetWorld(),HeroWidgetClass);
		if (HeroWidget) {
			HeroWidget->AddToViewport();
		}
	}
	HeroWidget->UpDateHealth(this);
	HeroWidget->UpDateStamina(this);
	//������������
	if (BackPackWidgetClass) {
		BackPackUI = CreateWidget<UBackPackWidget>(GetWorld(), BackPackWidgetClass);
		if (BackPackUI) {
			BackPackUI->AddToViewport();
			BackPackUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//��������󶨽�ɫ
	UInventoryMangerInstance* BPSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventoryMangerInstance>();
	BPSubsystem->Bind_Chara = this;
	////����ǹ
	//if (ConfigWeapon) {
	//	FActorSpawnParameters SpawnParams;
	//	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon);
	//	CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("hand_r_Weapon"));
	//	CurrentWeapon->SetActorHiddenInGame(false);//��ʱ�����أ��������������ٿ���
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
	//�л�����װ��
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
	//���л���ɶɶɶ-ռ��λ��
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
//����������Ʒ�������ǽ������������б����ṹȫ����ʼ������Ϣ�����ݱ����ṹ����Ϣ�������޸ģ��ﵽ��Ʒ��ӵ�Ч��
void AHero::AddItemToBackPack(const FBackPackStruct& NewItem) {
	int32 EmptySlotIndex = -1;

	if (NewItem.bIsStackable) {
		// ���ҿɶѵ���Ʒ
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
		// ���ɶѵ���Ʒֱ���ҿ�λ
		for (int32 i = 0; i < BackPackArray.Num(); i++) {
			if (BackPackArray[i].ItemID == 0) {
				BackPackArray[i] = NewItem;
				return;
			}
		}
	}

	// ����ɶѵ���û�ҵ���ͬ��Ʒ�����ÿ�λ
	if (EmptySlotIndex != -1) {
		BackPackArray[EmptySlotIndex] = NewItem;
	}
}
//ʰȡ����
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