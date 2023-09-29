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

	if (const UWorld* World = GetWorld())
	{
		//Get the destination vector by getting 1)Destination point and subtract it with the location of the body mesh
		//const FVector2D DeltaLocation = FVector2D{Value.X, Value.Y};
		const FVector2D DeltaLocation = FVector2D{1, 1};

		//Get Forward vector
		//FVector2D ForwardVector2D = FVector2D{BodyMesh->GetForwardVector().X, BodyMesh->GetForwardVector().Y};
		FVector2D ForwardVector2D = FVector2D{0, -1};

		//Create The FRotator from the two vectors
		double Angle = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(DeltaLocation, ForwardVector2D)));

		double DotProduct = (DeltaLocation.X * ForwardVector2D.X) + (DeltaLocation.Y * ForwardVector2D.Y);
		double Magnitude1 = DeltaLocation.Size();
		double Magnitude2 = ForwardVector2D.Size();
		double CosAlpha = FMath::Acos(DotProduct/Magnitude1*Magnitude2);

		UE_LOG(LogTemp, Warning, TEXT("Dot: %f"), DotProduct);
		UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), CosAlpha);
		if (Angle > 180.0f) Angle -= 360.0f;
		else if (Angle < -180.0f) Angle += 360.0f;
		//Actually rotate the tank an interpolated quantity of degrees
		FRotator Rotator(0, Angle / 100, 0);
		BodyMesh->AddLocalRotation(Rotator);

		//Move forward according the body mesh turret direction (THIS PART WORKS FINE)
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
