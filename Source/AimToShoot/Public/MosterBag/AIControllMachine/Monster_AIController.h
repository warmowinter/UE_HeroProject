// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Monster_AIController.generated.h"



class UBehaviorTree;
class UBlackboardData;
class AHero;
class APawn;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class AIMTOSHOOT_API AMonster_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonster_AIController();

protected:
	virtual void BeginPlay() override;
	//组件感知
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComp;

	//视觉感官
	UPROPERTY()
		UAISenseConfig_Sight* SightConfig;
		
	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 黑板 IsSeenPlayer 名称
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName IsSeenPlayerKey = "IsSeenPlayer";

private:
	//行为树
	UPROPERTY(EditAnywhere, Category = "AI")
		UBehaviorTree* BehaviorTreeAsset;
	//黑板Asset
	UPROPERTY(EditAnywhere, Category = "AI")
		UBlackboardData* BlackboardAsset;
	//BBcomp
	UPROPERTY()
		UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
		AHero* Hero;
	UPROPERTY(VisibleAnywhere, Category = "AI")
		APawn* ControlledPawn;
	UPROPERTY(VisibleAnywhere, Category = "AI")
		UCharacterMovementComponent* MoveComp;
};
