// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "TimerManager.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor){
    if (DeadActor == Tank) {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController) {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    } else if (ATower* DestroyedTower = Cast<ATower>(DeadActor)) {
        DestroyedTower->HandleDestruction();
        TargetTowers--;
        if (TargetTowers <= 0) {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay() {
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart() {
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    TargetTowers = GetTargetTowerCount();

    StartGame();

    if (ToonTanksPlayerController) {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableTimerHandle, InputDelegate, StartDelay, false);
    }

}

int32 AToonTanksGameMode::GetTargetTowerCount() {
    TArray<AActor*> TowerArray;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerArray);
    return TowerArray.Num();
}

