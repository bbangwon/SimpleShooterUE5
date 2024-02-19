// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooerCharacter::AShooerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enhanced Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> INPUT_MAPPING_CONTEXT(TEXT("/Game/Input/IMC_SimpleShooter"));
	if(INPUT_MAPPING_CONTEXT.Succeeded())
		InputMappingContext = INPUT_MAPPING_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_MOVE_FORWARD(TEXT("/Game/Input/IA_MoveForward"));
	if(INPUT_ACTION_MOVE_FORWARD.Succeeded())
		MoveForwardAction = INPUT_ACTION_MOVE_FORWARD.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_MOVE_RIGHT(TEXT("/Game/Input/IA_MoveRight"));
	if(INPUT_ACTION_MOVE_RIGHT.Succeeded())
		MoveRightAction = INPUT_ACTION_MOVE_RIGHT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_LOOK_UP(TEXT("/Game/Input/IA_LookUp"));
	if(INPUT_ACTION_LOOK_UP.Succeeded())
		LookUpAction = INPUT_ACTION_LOOK_UP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_LOOK_UP_RATE(TEXT("/Game/Input/IA_LookUpRate"));
	if(INPUT_ACTION_LOOK_UP_RATE.Succeeded())
		LookUpRateAction = INPUT_ACTION_LOOK_UP_RATE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_LOOK_RIGHT(TEXT("/Game/Input/IA_LookRight"));
	if(INPUT_ACTION_LOOK_RIGHT.Succeeded())
		LookRightAction = INPUT_ACTION_LOOK_RIGHT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_LOOK_RIGHT_RATE(TEXT("/Game/Input/IA_LookRightRate"));
	if(INPUT_ACTION_LOOK_RIGHT_RATE.Succeeded())
		LookRightRateAction = INPUT_ACTION_LOOK_RIGHT_RATE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_JUMP(TEXT("/Game/Input/IA_Jump"));
	if (INPUT_ACTION_JUMP.Succeeded())
		JumpAction = INPUT_ACTION_JUMP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_SHOOT(TEXT("/Game/Input/IA_Shoot"));
	if (INPUT_ACTION_SHOOT.Succeeded())
		ShootAction = INPUT_ACTION_SHOOT.Object;
}

// Called when the game starts or when spawned
void AShooerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//Enhanced Input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Enhanced Input
	//if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	//{
	//	EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AShooerCharacter::MoveForward_Enhanced);
	//	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AShooerCharacter::MoveRight_Enhanced);
	//	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AShooerCharacter::LookUp_Enhanced);
	//	EnhancedInputComponent->BindAction(LookUpRateAction, ETriggerEvent::Triggered, this, &AShooerCharacter::LookUpRate_Enhanced);
	//	EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &AShooerCharacter::LookRight_Enhanced);
	//	EnhancedInputComponent->BindAction(LookRightRateAction, ETriggerEvent::Triggered, this, &AShooerCharacter::LookRightRate_Enhanced);
	//	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooerCharacter::Jump_Enhanced);
	//	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShooerCharacter::Shoot_Enhanced);
	//}

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooerCharacter::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooerCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooerCharacter::Shoot);
}

void AShooerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooerCharacter::LookUp(float AxisValue)
{
	APawn::AddControllerPitchInput(AxisValue);
}

void AShooerCharacter::LookUpRate(float AxisValue)
{
	LookUp(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooerCharacter::LookRight(float AxisValue)
{
	APawn::AddControllerYawInput(AxisValue);
}

void AShooerCharacter::LookRightRate(float AxisValue)
{
	LookRight(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooerCharacter::Shoot()
{
	if (Gun)
	{
		Gun->PullTrigger();	
	}
}

void AShooerCharacter::MoveForward_Enhanced(const FInputActionValue& Value)
{
	MoveForward(Value.Get<float>());
}

void AShooerCharacter::MoveRight_Enhanced(const FInputActionValue& Value)
{
	MoveRight(Value.Get<float>());
}

void AShooerCharacter::LookUp_Enhanced(const FInputActionValue& Value)
{
	LookUp(Value.Get<float>());
}

void AShooerCharacter::LookUpRate_Enhanced(const FInputActionValue& Value)
{
	LookUpRate(Value.Get<float>());
}

void AShooerCharacter::LookRight_Enhanced(const FInputActionValue& Value)
{
	LookRight(Value.Get<float>());
}

void AShooerCharacter::LookRightRate_Enhanced(const FInputActionValue& Value)
{
	LookRightRate(Value.Get<float>());
}

void AShooerCharacter::Jump_Enhanced(const FInputActionValue& Value)
{
	Jump();
}

void AShooerCharacter::Shoot_Enhanced(const FInputActionValue& Value)
{
	Shoot();
}

float AShooerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageApplied;
}

bool AShooerCharacter::IsDead() const
{
	return Health <= 0.f;
}
