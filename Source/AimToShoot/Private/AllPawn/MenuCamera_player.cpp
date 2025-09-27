// Fill out your copyright notice in the Description page of Project Settings.


#include "AllPawn/MenuCamera_player.h"
#include "Camera/CameraComponent.h"


// Sets default values
AMenuCamera_player::AMenuCamera_player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//camera Create and some config
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MenuCamera"));
	RootComponent = PlayerCamera;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMenuCamera_player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMenuCamera_player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMenuCamera_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

