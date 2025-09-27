// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuCamera_player.generated.h"


class UCameraComponent;

UCLASS()
class AIMTOSHOOT_API AMenuCamera_player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuCamera_player();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* PlayerCamera;




};
