// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletsBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MosterBag/MosterBase.h"
#include "WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GamePlayStatics.h"
// Sets default values
ABulletsBase::ABulletsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//设置碰撞组件初始化
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABulletsBase::OnHit);
	RootComponent = CollisionComp;
	//mesh
	BullMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BullMesh"));
	BullMeshComp->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	//3s后销毁
	InitialLifeSpan = 3.0f;
	//被击中者初始化

	Is_Hited_Moster = nullptr;
	Bind_Weapon = nullptr;
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

void ABulletsBase::FireInDirection(const FVector& ShootDirection) {
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void ABulletsBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor && OtherActor != this && OtherActor != GetOwner()) {
		/*UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetInstigatorController(), this, nullptr);*/
		Bind_Weapon->NoticeAHeroAttackInfo(Cast<AMosterBase>(OtherActor), Hit);
		Destroy();
	}
}
