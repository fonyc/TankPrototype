// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPrototype/Public/SBasePawn.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASBasePawn::ASBasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshGroup = CreateDefaultSubobject<USceneComponent>(TEXT("MeshGroup"));
	MeshGroup->SetupAttachment(RootComponent);

	BodyMesh = CreateDefaultSubobject<USceneComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(MeshGroup);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(MeshGroup);

	TurretTurnSpeed = 25.0f;
}

// Called when the game starts or when spawned
void ASBasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ASBasePawn::RotateTurret(const FVector& TargetRotation) const
{
	//BUG Adding a 90 degree offset to spin cause turret mesh is WRONG!!!  
	const FVector Target = TargetRotation - TurretMesh->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.0f, Target.Rotation().Yaw - 90.0f, 0.0f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			TurretTurnSpeed));
}

// Called every frame
void ASBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
