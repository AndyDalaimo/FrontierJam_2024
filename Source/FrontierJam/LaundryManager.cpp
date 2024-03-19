// Fill out your copyright notice in the Description page of Project Settings.


#include "LaundryManager.h"

#include "TimerManager.h"

ALaundryManager::ALaundryManager() : timerRate(2.f), timerDecreaseIncrement(.1f)
{
	
}


void ALaundryManager::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());
	ShopManagerRef = Cast<AShopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AShopManager::StaticClass()));

	TimerDelegate.BindUFunction(this, "LaundrySpawnTimerHandler");
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, TimerDelegate, timerRate, true);
}

void ALaundryManager::InitializeLaundrySpawnTimer()
{
	TimerDelegate.BindUFunction(this, "LaundrySpawnTimerHandler");
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, TimerDelegate, timerRate, true);
}


void ALaundryManager::LaundrySpawnTimerHandler()
{
	if (GameInstanceRef->GameState == EGameState::DAY)
	{
		BagReferences.Push(Cast<ALaundryBag>(GetWorld()->SpawnActor<ALaundryBag>(GetLaundrySpawnLocation(), FRotator(0,0,0), params)));
	}
	else {
		ReputationUpdate();
		GameInstanceRef->GetTimerManager().ClearTimer(SpawnTimer);
		AdjustTimerRate();
		// timerRate = FMath::Clamp(timerRate -= timerDecreaseIncrement, 1.0f, 10.f);

	}
}

// Return one of the possible spawn locations for the laundry bags
FVector ALaundryManager::GetLaundrySpawnLocation()
{
	if (!PossibleSpawnLocations.IsEmpty())
	{
		int RandomLocationIndex = FMath::RandRange(0, PossibleSpawnLocations.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Random Location Index: %d"), RandomLocationIndex);
		return PossibleSpawnLocations[RandomLocationIndex];
	}
	else {
		return FVector(0, 0, 0);
	}
}

// Take One less reputation point for each bag of Laundry the player 
// did not get to servicing 
void ALaundryManager::ReputationUpdate()
{
	int numBagsLeft = 0;
	for (ALaundryBag* bag : BagReferences)
	{
		if (IsValid(bag))
		{
			numBagsLeft++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Num of Bag References end of Day: %d"), numBagsLeft);
	ShopManagerRef->Reputation.Reputation -= numBagsLeft;
	ShopManagerRef->UpdateReputation();


}

void ALaundryManager::DestroyAllLeftoverBags()
{
	for (ALaundryBag* bag : BagReferences)
	{
		if (IsValid(bag))
		{
			bag->Destroy();
		}
	}

	BagReferences.Empty();
}

// Adjust the amount of laundry the Shop gets depending on its reputation state
void ALaundryManager::AdjustTimerRate()
{

	switch (ShopManagerRef->Reputation.RepState)
	{
		case (EReputationState::NEGATIVE) :
			timerRate = 8.f;
			break;
		case (EReputationState::NEUTRAL) :
			timerRate = 6.f;
			break;
		case (EReputationState::POSITIVE) :
			timerRate = 4.f;
		case (EReputationState::EXCELLENT) :
			timerRate = 3.f;
	}
}
