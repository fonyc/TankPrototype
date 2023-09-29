// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementTypeInterface.generated.h"

UINTERFACE()
class UMovementTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKPROTOTYPE_API IMovementTypeInterface
{
	GENERATED_BODY()

	virtual void Move() = 0;
	virtual void Turn() = 0;

public:
};
