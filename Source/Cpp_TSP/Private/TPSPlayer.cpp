// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Engine/SkeletalMesh.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200;
	SpringArmComp->SetRelativeLocation(FVector(0, 40, 70));

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 절대적인 방향을 나를 기준으로 회전하고싶다.
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::OnIAMove);
	Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATPSPlayer::OnIALook);
	Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATPSPlayer::OnIAJump);

}

void ATPSPlayer::OnIAMove(const FInputActionValue& value)
{
	auto vec = value.Get<FVector2D>();
	Direction.X = vec.X;
	Direction.Y = vec.Y;
}

void ATPSPlayer::OnIALook(const FInputActionValue& value)
{
	auto vec = value.Get<FVector2D>();
	AddControllerPitchInput(vec.Y);
	AddControllerYawInput(vec.X);
}

void ATPSPlayer::OnIAJump(const FInputActionValue& value)
{
	Jump();
}

