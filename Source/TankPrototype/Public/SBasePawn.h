// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SBasePawn.generated.h"

class UBoxComponent;

UCLASS()
class TANKPROTOTYPE_API ASBasePawn : public APawn
{
	GENERATED_BODY()

public:
	ASBasePawn();

protected:
	virtual void BeginPlay() override;

	void RotateTurret(const FVector& TargetRotation) const;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	USceneComponent* BodyMesh;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	UBoxComponent* BoxComponent;

	//Scene component used to group every mesh component since we dont have a SK for the tank
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	USceneComponent* MeshGroup;

	//Refers to the piece of mesh that is going to rotate with the mouse
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, meta=(Category = "Movement", AllowPrivateAccess = true))
	float TurretTurnSpeed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
