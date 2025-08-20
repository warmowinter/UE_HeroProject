// Fill out your copyright notice in the Description page of Project Settings.


#include "MosterBag/AIControllMachine/Monster_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Hero.h"
#include "UObject/ConstructorHelpers.h"


AMonster_AIController::AMonster_AIController() {
	//感知组件配置
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	//类视觉感官配置
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 2000.f;
	SightConfig->PeripheralVisionAngleDegrees = 75.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

	//绑定委托-感知更新
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMonster_AIController::OnTargetPerceptionUpdated);
	
	//load行为树资源
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("BehaviorTree'/Game/Current_make/Moster_bag/UEC++_AI/Monster_BehaviorTree.Monster_BehaviorTree'")
	);
	if (BTObject.Succeeded()) {
		BehaviorTreeAsset = BTObject.Object;
	}

	//load黑板资源
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("BlackboardData'/Game/Current_make/Moster_bag/UEC++_AI/BlackBoard_Data.BlackBoard_Data'")
	);
	if (BBObject.Succeeded()) {
		BlackboardAsset = BBObject.Object;
	}



}



void AMonster_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardAsset) {
		UseBlackboard(BlackboardAsset, BlackboardComponent);
	}

	if (BehaviorTreeAsset) {
		RunBehaviorTree(BehaviorTreeAsset);
	}

	//初始化被控制Actor类

	ControlledPawn = GetPawn();
	MoveComp = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>();
	MoveComp->MaxWalkSpeed = 200.f;
}

void AMonster_AIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!BlackboardComponent) {
		return;
	}

	Hero = Cast<AHero>(Actor);
	if (Hero) {
		bool sfs = Stimulus.WasSuccessfullySensed();
		BlackboardComponent->SetValueAsBool(IsSeenPlayerKey, sfs);


		if (ControlledPawn) {
			if (MoveComp) {
				if (sfs) {
					MoveComp->MaxWalkSpeed = 500.f;
				}
				else {
					MoveComp->MaxWalkSpeed = 200.f;
				}
			}
		}
	}


	//if (Stimulus.WasSuccessfullySensed()) {
	//	GetBlackboardComponent()->SetValueAsObject(IsSeenPlayerKey, Actor);
	//}
	//else {
	//	GetBlackboardComponent()->ClearValue(IsSeenPlayerKey);
	//}
}
