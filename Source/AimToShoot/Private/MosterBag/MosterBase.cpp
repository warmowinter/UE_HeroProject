// Fill out your copyright notice in the Description page of Project Settings.


#include "MosterBag/MosterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Hero.h"
#include "BulletsBase.h"
#include "MosterBag/AIControllMachine/Monster_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AMosterBase::AMosterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MonsterMesh = GetMesh();
	MonsterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MonsterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(150.f);
	SphereComp->SetCollisionProfileName(TEXT("Pawn"));

	MaxHealth = 20.0f;
	CurrentHealth = MaxHealth;
	MaxWalk = 300.0f;
	AttackBase = 10.0f;
	IsDead = false;
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


	DoSphereTrace();

	if (AMonster_AIController* AICon = Cast<AMonster_AIController>(this->GetController())) {
		if (UBlackboardComponent* BBComp = AICon->GetBlackboardComponent()) {
			bool bSeePlayer = BBComp->GetValueAsBool("IsSeenPlayer");
			if (bSeePlayer && !IsDead && EnemyIsNearby) {
				IsAttack = true;
			}
			else {
				IsAttack = false;
			}
		}
	}
}

// Called to bind functionality to input
void AMosterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMosterBase::HandleAttacked(AHero* Attacker, AMosterBase* Victim, const FHitResult& Hit)
{
	if (this == Victim) {

		CurrentHealth -= Attacker->CurrentWeapon->GetBaseDamage() * 5.f;
		if (CurrentHealth <= 0) {
			IsDead = true;
			MosterDie();
		}
		UE_LOG(LogTemp, Log, TEXT("damage CurrentHealth:%d"),CurrentHealth);
	}
}
void AMosterBase::MosterDie() {
	//可以考虑搞个特效，等后期吧
	AController* Mon_Controller = GetController();
	if (Mon_Controller) {
		Mon_Controller->StopMovement();
		Mon_Controller->UnPossess();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DeathMontage) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			//AnimInstance->Montage_Play(DeathMontage);
			float MontageDuration = AnimInstance->Montage_Play(DeathMontage);
			if (MontageDuration > 0.f) {
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, this, &AMosterBase::OnDeathAnimationFinished,MontageDuration,false);

			}
		}
	}
	else {
		Destroy();
	}
}
//死亡延迟
void AMosterBase::OnDeathAnimationFinished()
{
	SetLifeSpan(1.0f);
}


void AMosterBase::ReceiveHitFrom(ACharacter* Attacker)
{
	//后续等做受击检测
}

void AMosterBase::OnNearbyCombat(ACharacter* Attacker, AMosterBase* Victim)
{
	//ռλ
}

void AMosterBase::DoSphereTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 40.f;
	float Radius = 60.f;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForOneFrame;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		Radius,
		TraceChannel,
		false,
		ActorsToIgnore,
		DrawDebugType,
		HitResult,
		true
	);


	if (bHit) {
		AActor* HitActor = Cast<AHero>(HitResult.GetActor());
		if (HitActor) {
			 EnemyIsNearby = true;
		}
	}
	else {
		EnemyIsNearby = false;
	}
}

bool AMosterBase::GetMonsterAttackState()
{
	return IsAttack;
}

