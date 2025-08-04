// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletsBase.h"

// Sets default values
ABulletsBase::ABulletsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

