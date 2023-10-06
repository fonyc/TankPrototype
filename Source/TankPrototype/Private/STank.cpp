// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPrototype/Public/STank.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ASTank::ASTank()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//Default settings-> Overriden in Editor
	SpringArmComponent->TargetArmLength = 2500.0f;
	SpeedMultiplier = 1000.0f;
	BodyTurnSpeed = 2.0f;
}

void ASTank::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerControllerRef = PlayerController;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void ASTank::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Destination %f,%f"), Value.X, Value.Y);
	//Get the destination vector by getting 1)Destination point and subtract it with the location of the body mesh
	FVector DestinationVector = FVector{Value.X, Value.Y, 0};

	//Get Forward vector
	FVector ForwardVector = FVector{BodyMesh->GetForwardVector().X, BodyMesh->GetForwardVector().Y, 0};
	UE_LOG(LogTemp, Warning, TEXT("FW %f,%f"), BodyMesh->GetForwardVector().X, BodyMesh->GetForwardVector().Y);

	//Create The FRotator from the two vectors
	Angle = FMath::RadiansToDegrees(FMath::Cos(FVector::DotProduct(DestinationVector, ForwardVector)));
	UE_LOG(LogTemp, Warning, TEXT("Acos %f"), FMath::Acos(FVector::DotProduct(DestinationVector, ForwardVector)));

	//Get Angle direction 
	FVector CrossProduct = FVector::CrossProduct(DestinationVector, ForwardVector);
	float Sign = CrossProduct.Z < 0 ? 1 : -1;

	DestinationRotator = FRotator(0, Sign * Angle, 0);

	if (UWorld* World = GetWorld())
	{
		BodyMesh->AddLocalRotation(DestinationRotator * BodyTurnSpeed * UGameplayStatics::GetWorldDeltaSeconds(World));

		//Move forward according the body mesh turret direction
		AddActorLocalOffset(
			BodyMesh->GetForwardVector() * SpeedMultiplier * UGameplayStatics::GetWorldDeltaSeconds(World));
	}
}

void ASTank::MouseTrace() const
{
	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);
		DrawDebugSphere(GetWorld(), HitResult.Location, 50.0f, 12, FColor::Green, false, -1.0f);
		RotateTurret(HitResult.ImpactPoint);
	}
}

void ASTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MouseTrace();
}

//HOVER MOVEMENT
// void ASTank::HoverMove(const FInputActionValue& InputActionValue)
// {
// 	const FVector2D Value = InputActionValue.Get<FVector2D>();
// 	
// 	if (const UWorld* World = GetWorld())
// 	{
//
// 		const FVector DeltaLocation = FVector
// 		{
// 			Value.X * SpeedMultiplier * UGameplayStatics::GetWorldDeltaSeconds(World),
// 			Value.Y * SpeedMultiplier * UGameplayStatics::GetWorldDeltaSeconds(World),
// 			0.0f
// 		};
// 		
// 		AddActorLocalOffset(DeltaLocation);
//
// 		//Here we should add the rotation of the body mesh but hover tanks do not need it
//		//const FRotator NewRotation(0, .15f, 0); 
//		//BodyMesh->AddLocalRotation(NewRotation);
// 	}	
// }


void ASTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTank::Move);
}
