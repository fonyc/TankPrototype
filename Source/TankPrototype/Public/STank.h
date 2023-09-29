// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SBasePawn.h"
#include "STank.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TANKPROTOTYPE_API ASTank : public ASBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, meta=(Categoty = "Movement", AllowPrivateAccess = true))
	float SpeedMultiplier;

private:
	APlayerController* PlayerControllerRef;

	//INPUT METHODS
	void Move(const FInputActionValue& InputActionValue);
	void MouseTrace() const;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<UObject> MovementType;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> Context;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
