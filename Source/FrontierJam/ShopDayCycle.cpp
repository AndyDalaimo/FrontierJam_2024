// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopDayCycle.h"

#include "TimerManager.h"



UShopDayCycle::UShopDayCycle(const FObjectInitializer& ObjectInitializer) : dayCycleTime(10), timerRate(1.f), GameState(EGameState::DAY), TimeLeftInDay(dayCycleTime)
{

}


// Start Day Cycle
// Player will interact with world during this cycle. Will Restart when player decides to end Night Cycle
void UShopDayCycle::InitializeDayCycle()
{
	PlayerPawnRef = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerPawnRef->SetActorLocation(PlayerStartPosition);
	PlayerPawnRef->SetActorRotation(PlayerStartRotation, ETeleportType::ResetPhysics);

	timeElapsed = 0.f;
	GameState = EGameState::DAY;

	TimerDelegate.BindUFunction(this, "DayCycle");
	GetWorld()->GetTimerManager().SetTimer(DayTimer, TimerDelegate, timerRate, true);
	K2_OnDayCycle();
}

// Function ticking on timerRate.
// Clears timer and initiate Night Cycle when dayCycleTime Ends
void UShopDayCycle::DayCycle()
{
	timeElapsed++;
	TimeLeftInDay = dayCycleTime - timeElapsed;
	// UE_LOG(LogTemp, Display, TEXT("Seconds elapsed: %f"), timeElapsed);

	if (timeElapsed >= dayCycleTime)
	{
		GetTimerManager().ClearTimer(DayTimer);
		GameState = EGameState::NIGHT;
		K2_OnNightCycle();
	}
}
