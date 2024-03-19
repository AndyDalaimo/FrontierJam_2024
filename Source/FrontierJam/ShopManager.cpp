// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopManager.h"

AShopManager::AShopManager()
{
	// Initialize Base store Economy and Reputation Values
	Economy.init();
	Reputation.init();
	ShopState = EShopState::GOODSTANDING;
}


// ----------------------------------------------------------
// ---------------- Economy Helper Functions ----------------
// ----------------------------------------------------------

// Buy Item: Subtract from total Cash in Economy
void AShopManager::BuyItem(float cost)
{
	if (Economy.Cash >= cost) Economy.Cash -= cost;
	else UE_LOG(LogTemp, Warning, TEXT("Can't Buy that!"));
}

// Subtract Utilities from the Shop Economy at the start of each day, If they do not have enough money, Fail state
EShopState AShopManager::PayUtilities()
{
	UE_LOG(LogTemp, Warning, TEXT("Paying Utilities"));
	Economy.Cash -= Economy.UtilitiesCost;

	if (Economy.Cash < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, "Fail State");
		ShopState = EShopState::FAIL;
	}

	return ShopState;
}

// 
void AShopManager::UpdateUtilitiesCost(EMachineUpgrade MachineState)
{
	int temp = 0;

	Economy.CurrentMachines.Push(MachineState);

	for (EMachineUpgrade machine : Economy.CurrentMachines)
	{
		temp += static_cast<int32>(machine);
		// UE_LOG(LogTemp, Warning, TEXT("Current Machine: %d"), static_cast<int32>(machine));
	}

	Economy.UtilitiesCost = temp * Economy.CurrentMachines.Num();
}


// ----------------------------------------------------------
// --------------- Reputation Helper Functions --------------
// ----------------------------------------------------------

void AShopManager::UpdateCleanliness(int8 update)
{
	Reputation.Cleanliness += update;
}

void AShopManager::UpdateReputation()
{
	// Reputation.Reputation += (Reputation.Cleanliness + Reputation.Decor) * static_cast<int32>(Reputation.RepState);


	// Change this later. Is ugly
	if (Reputation.Reputation > 20)
	{
		Reputation.RepState = EReputationState::POSITIVE;
	}
	else if (Reputation.Reputation > 50)
	{
		Reputation.RepState = EReputationState::EXCELLENT;
	}
	else if (Reputation.Reputation > 0)
	{
		Reputation.RepState = EReputationState::NEUTRAL;
	}
	else {
		Reputation.RepState = EReputationState::NEGATIVE;
	}
}

