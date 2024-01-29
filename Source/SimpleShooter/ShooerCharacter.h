// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooerCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRight(float AxisValue);
	void LookRightRate(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 70.f;

	// For Enhanced Input
	class UInputMappingContext* InputMappingContext;
	class UInputAction* MoveForwardAction;
	class UInputAction* MoveRightAction;
	class UInputAction* LookUpAction;
	class UInputAction* LookUpRateAction;
	class UInputAction* LookRightAction;
	class UInputAction* LookRightRateAction;
	class UInputAction* JumpAction;

	void MoveForward_Enhanced(const struct FInputActionValue& Value);
	void MoveRight_Enhanced(const struct FInputActionValue& Value);	
	void LookUp_Enhanced(const struct FInputActionValue& Value);
	void LookUpRate_Enhanced(const struct FInputActionValue& Value);
	void LookRight_Enhanced(const struct FInputActionValue& Value);
	void LookRightRate_Enhanced(const struct FInputActionValue& Value);
	void Jump_Enhanced(const struct FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	UPROPERTY()
	AGun* Gun;
};
