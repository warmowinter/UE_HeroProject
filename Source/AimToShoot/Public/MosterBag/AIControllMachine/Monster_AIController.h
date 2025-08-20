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
	//�����֪
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UAIPerceptionComponent* AIPerceptionComp;

	//�Ӿ��й�
	UPROPERTY()
		UAISenseConfig_Sight* SightConfig;
		
	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// �ڰ� IsSeenPlayer ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName IsSeenPlayerKey = "IsSeenPlayer";

private:
	//��Ϊ��
	UPROPERTY(EditAnywhere, Category = "AI")
		UBehaviorTree* BehaviorTreeAsset;
	//�ڰ�Asset
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
