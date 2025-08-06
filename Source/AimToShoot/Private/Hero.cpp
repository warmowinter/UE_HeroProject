// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
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
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//启用角色朝移动方向转向,且配置转向速率
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//set mesh location and rotation设置位置和旋转
	MyMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MyMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//Springboom
	SpringRob = CreateDefaultSubobject<USpringArmComponent>(TEXT("CaremaBoom"));
	SpringRob->SetupAttachment(RootComponent);
	SpringRob->TargetArmLength = 400.0f;
	SpringRob->bUsePawnControlRotation = true;
	//CameraComponent
	MyCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	MyCameraComponent->SetupAttachment(SpringRob,USpringArmComponent::SocketName);
	MyCameraComponent->bUsePawnControlRotation = false;


	SpringRob_2D = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom2"));
	SpringRob_2D->SetupAttachment(RootComponent);
	SpringRob_2D->TargetArmLength = 1200.0f;
	SpringRob_2D->bUsePawnControlRotation = false;//弹簧臂不跟随控制器旋转而旋转
	SpringRob_2D->SetUsingAbsoluteRotation(true);//设置世界绝对旋转，不跟随角色旋转而旋转


	MyCameraComponent_2D = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp2"));
	MyCameraComponent_2D->SetupAttachment(SpringRob_2D, USpringArmComponent::SocketName);
	MyCameraComponent_2D->bUsePawnControlRotation = false;
	


	CurrentCamera = MyCameraComponent;//设置当前摄像机
	MyCameraComponent->SetActive(true);//激活第一个摄像机
	MyCameraComponent_2D->SetActive(false);


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
	//创建生命和体力值界面
	if (HeroWidgetClass) {
		HeroWidget = CreateWidget<UHero_Widget>(GetWorld(),HeroWidgetClass);
		if (HeroWidget) {
			HeroWidget->AddToViewport();
		}
	}
	HeroWidget->UpDateHealth(CurrentHealth/MaxHealth);
	HeroWidget->UpDateStamina(CurrentStamina/ MaxStamina);
	//配置枪
	if (ConfigWeapon) {
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(ConfigWeapon);
		CurrentWeapon->AttachToComponent(MyMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("hand_r_Weapon"));
		CurrentWeapon->SetActorHiddenInGame(false);//暂时不隐藏，后续背包出来再开发
	}


}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Is_Run && CurrentStamina) {
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		HeroWidget->UpDateStamina(CurrentStamina/MaxStamina);
		if (CurrentStamina <= 0) {
			CurrentStamina = 0;
			StopFastRun();
		}
	}
	if (Is_Run == false && CurrentStamina <= MaxStamina) {
		HeroWidget->UpDateStamina(CurrentStamina / MaxStamina);
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


	//move
	PlayerInputComponent->BindAxis("Move Forward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AHero::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AHero::LookUpRate);

	//toggle view
	PlayerInputComponent->BindAction("ToggleView", IE_Pressed, this, &AHero::ToggleCharacterView);


}

void AHero::MoveForward(float value) {
	if (Controller != nullptr && value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AHero::MoveRight(float value) {
	if (Controller != nullptr && value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AHero::TurnAtRate(float rate) {
	AddControllerYawInput(rate * TurnRateBase * GetWorld()->GetDeltaSeconds());
}

void AHero::LookUpRate(float rate) {
	AddControllerPitchInput(rate * TurnRateBase * GetWorld()->GetDeltaSeconds());
}


void AHero::ToggleCharacterView() {
	CurrentCamera = (CurrentCamera == MyCameraComponent) ? MyCameraComponent_2D : MyCameraComponent;
	MyCameraComponent->SetActive(CurrentCamera == MyCameraComponent);
	MyCameraComponent_2D->SetActive(CurrentCamera == MyCameraComponent_2D);

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
	HeroWidget->UpDateHealth(CurrentHealth/MaxHealth);

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
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AHero::StartFire() {
	if (CurrentWeapon) {
		CurrentWeapon->WeaponStartFire();
	}
}
void AHero::StopFire() {
	if (CurrentWeapon) {
		CurrentWeapon->WeaponStopFire();
	}
}
