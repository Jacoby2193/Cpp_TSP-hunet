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

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Gun;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Sniper;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Zoom;

	UPROPERTY(EditAnywhere)
	class UInputAction* IA_Fire;

	void OnIAMove(const FInputActionValue& value);
	void OnIALook(const FInputActionValue& value);
	void OnIAJump(const FInputActionValue& value);
	void OnIAGun(const FInputActionValue& value);
	void OnIASniper(const FInputActionValue& value);
	void OnIAZoomIn(const FInputActionValue& value);
	void OnIAZoomOut(const FInputActionValue& value);
	void OnIAFire(const FInputActionValue& value);

	FVector Direction;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SniperMesh;

	bool bChooseSniperGun = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	class UUserWidget* CrosshairUI;

	UPROPERTY()
	class UUserWidget* SniperUI;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> BulletFactory;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFXFactory;

	// 총을 쏘면 움찔하는 몽타주를 재생하고싶다.
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FireAnimMontage;

	// 카메라 진동처리에 필요한 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
	class USoundBase* BulletSound;

	UPROPERTY(EditAnywhere)
	int32 MaxHP = 3;

	UPROPERTY(EditAnywhere)
	int32 HP = MaxHP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HpUIFactory;

	class UPlayerHPUI* HpUI;

	void OnMyTakeDamage();

	UPROPERTY( EditAnywhere )
	TSubclassOf<class UUserWidget> GameOverUIFactory;


};
