// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopManager.h"

AShopManager::AShopManager()
{
	// Initialize Base store Economy and Reputation Values
	Economy.init();
	Reputation.init();
}


// ----------------------------------------------------------
// ---------------- Economy Helper Functions ----------------
// ----------------------------------------------------------

// Buy Item: Subtract from total Cash in Economy
void AShopManager::BuyItem(float cost)
{
	Economy.Cash -= cost;
}

// 
void AShopManager::UpdateUtilitiesCost(TArray<EMachineUpgrade> Machines, int NumMachines)
{
	int temp = 0;

	for (EMachineUpgrade machine : Machines)
	{
		temp += static_cast<int32>(machine);
	}

	Economy.UtilitiesCost = temp * NumMachines;
}



// ----------------------------------------------------------
// --------------- Reputation Helper Functions --------------
// ----------------------------------------------------------

void AShopManager::UpdateCleanliness(int8 update)
{
	Reputation.Cleanliness += update;
}