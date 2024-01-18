// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_LOOK_RIGHT(TEXT("/Game/Input/IA_LookRight"));
	if(INPUT_ACTION_LOOK_RIGHT.Succeeded())
		LookRightAction = INPUT_ACTION_LOOK_RIGHT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> INPUT_ACTION_JUMP(TEXT("/Game/Input/IA_Jump"));
	if (INPUT_ACTION_JUMP.Succeeded())
		JumpAction = INPUT_ACTION_JUMP.Object;
}

// Called when the game starts or when spawned
void AShooerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Enhanced Input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}	
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
	//	EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &AShooerCharacter::LookRight_Enhanced);
	//	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooerCharacter::Jump_Enhanced);
	//}

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}

void AShooerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
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
	APawn::AddControllerPitchInput(Value.Get<float>());
}

void AShooerCharacter::LookRight_Enhanced(const FInputActionValue& Value)
{
	APawn::AddControllerYawInput(Value.Get<float>());
}

void AShooerCharacter::Jump_Enhanced(const FInputActionValue& Value)
{
	Jump();
}
