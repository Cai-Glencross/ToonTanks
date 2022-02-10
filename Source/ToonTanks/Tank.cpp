// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#define OUT


ATank::ATank() {
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());

    
}

void ATank::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (TankPlayerController) {
        FHitResult HitResult; 
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false, 
            OUT HitResult
        );

        // DrawDebugSphere(GetWorld(), 
        //     HitResult.Location, 
        //     10.f,
        //     12,
        //     FColor::Red
        // );

        RotateTurret(HitResult.Location);
    }
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value) {
    //UE_LOG(LogTemp, Warning, TEXT("Value passed is %f"), Value);
    FVector DeltaLocation(Value*Speed*UGameplayStatics::GetWorldDeltaSeconds(this), 0.f, 0.f);
    AddActorLocalOffset(
        DeltaLocation, 
        true //bSweep
    );

} 

void ATank::Turn(float Value) {
    FRotator DeltaRotator = FRotator::ZeroRotator;
    DeltaRotator.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
    AddActorLocalRotation(
        DeltaRotator, 
        true //bSweep
    );
}

void ATank::HandleDestruction() {
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

    bAlive = false;
}