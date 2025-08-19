// Fill out your copyright notice in the Description page of Project Settings.


#include "MosterBag/MosterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Hero.h"
#include "BulletsBase.h"


// Sets default values
AMosterBase::AMosterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;




	MaxHealth = 20.0f;
	CurrentHealth = MaxHealth;
	MaxWalk = 300.0f;
	AttackBase = 10.0f;
}

// Called when the game starts or when spawned
void AMosterBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player) {
		Player->OnMosterAttacked.AddUniqueDynamic(this,&AMosterBase::HandleAttacked);
	}

}

void AMosterBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMosterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMosterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMosterBase::HandleAttacked(ACharacter* Attacker, AMosterBase* Victim, const FHitResult& Hit)
{
	if (this == Victim) {
		CurrentHealth -= 10;
		if (CurrentHealth <= 0) {
			MosterDie();
		}
		UE_LOG(LogTemp, Log, TEXT("damage CurrentHealth%"));
	}
}
void AMosterBase::MosterDie() {
	//可以考虑搞个特效，等后期吧
	Destroy();
}


void AMosterBase::ReceiveHitFrom(ACharacter* Attacker)
{
	//ռλ
}

void AMosterBase::OnNearbyCombat(ACharacter* Attacker, AMosterBase* Victim)
{
	//ռλ
}

