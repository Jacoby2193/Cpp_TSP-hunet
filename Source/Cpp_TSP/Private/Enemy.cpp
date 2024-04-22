// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "NavigationInvokerComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 에셋을 로드해서 메시에 적용하고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Models/Enemy/Model/vampire_a_lusth.vampire_a_lusth'" ) );

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh( TempMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0 , 0 , -90 ) , FRotator(0, -90, 0) );
	}


	ConstructorHelpers::FClassFinder<UEnemyAnim> TempAnim( TEXT( "/Script/Engine.AnimBlueprint'/Game/Blueprints/WBP_Enemy.WBP_Enemy_C'" ) );

	if (TempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass( TempAnim.Class );
	}

	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>( TEXT("NavInvokerComp"));
	NavInvokerComp->SetGenerationRadii( 500 , 800 );
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}

void AEnemy::OnMyTakeDamage( int damage )
{
	FSM->OnDamageProcess( damage );
}

