// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "IDLE") ,
	Move UMETA(DisplayName = "MOVE") ,
	Attack UMETA(DisplayName = "ATTACK") ,
	Damage UMETA(DisplayName = "DAMAGE") ,
	Die UMETA(DisplayName = "DIE") ,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_TSP_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	EEnemyState State;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	UPROPERTY()
	class ATPSPlayer* Target;

	UPROPERTY()
	class AEnemy* Me;

	UPROPERTY()
	class UEnemyAnim* EnemyAnim;

	void OnHit();

	void OnAttackEnd();

	void SetState(EEnemyState newState);

	float CurrentTime;

	UPROPERTY( EditAnywhere )
	float AttackRange = 150;

	UPROPERTY(EditAnywhere)
	float FindTime = 1;

	UPROPERTY(EditAnywhere)
	float AttackDelayTime = 2;

	UPROPERTY( EditAnywhere )
	float DamageDelayTime = 2;

	UPROPERTY( EditAnywhere )
	float DieDelayTime = 2;

	void OnDamageProcess( int damage );

	int HP = 2;

	bool bDieDone;

};
