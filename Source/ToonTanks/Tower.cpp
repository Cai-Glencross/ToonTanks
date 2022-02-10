// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Tank.h"

void ATower::BeginPlay() {
    Super::BeginPlay();
    //UE_LOG(LogTemp, Warning, TEXT("RUNNING BeginPlay"));
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("RUNNING TICK"));

    if (InFireRange()) {
        RotateTurret(Tank->GetActorLocation());
    }

}

void ATower::CheckFireCondition() {
    if (!Tank) {
        return;
    }
    if (InFireRange() && Tank->bAlive) {
        Fire();
    }
}

bool ATower::InFireRange() {
    if (Tank) {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        //UE_LOG(LogTemp, Warning, TEXT("Distance is: %f"), Distance);
        if (Distance <= FireRange) {
            return true;
        }
    }

    return false;
}
void ATower::HandleDestruction() {
    Super::HandleDestruction();

    Destroy();
}


