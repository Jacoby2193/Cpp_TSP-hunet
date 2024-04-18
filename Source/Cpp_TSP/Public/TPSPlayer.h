// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSPlayer.generated.h"

UCLASS()
class CPP_TSP_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 카메라와 스프링암
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CamComp;

	// IMC, IA
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Look;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Jump;

	void OnIAMove(const FInputActionValue& value);
	void OnIALook(const FInputActionValue& value);
	void OnIAJump(const FInputActionValue& value);

	FVector Direction;

};
