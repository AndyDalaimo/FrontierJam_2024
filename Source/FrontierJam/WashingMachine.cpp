// Fill out your copyright notice in the Description page of Project Settings.


#include "WashingMachine.h"

#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

// Sets default values
AWashingMachine::AWashingMachine() : UpgradeCost(100), UpgradeIncrease(250), UpgradeState(EMachineUpgrade::SMALL), ChaosState(EChaosState::WORKING),
WashCycleTime(5), bWashing(false), ReputationIncrease(1), ChoasChancePercent(.10f), ChoasChancePercentDecrease(0.01f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MachineMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
	MachineMeshRef = MachineMeshAsset.Object;

	MachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MachineMesh"));
	MachineMesh->SetStaticMesh(MachineMeshRef);

	RootComponent = MachineMesh;
	
	MachineCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MachineCollisionComp"));
	MachineCollisionComp->AttachToComponent(MachineMesh, FAttachmentTransformRules::KeepRelativeTransform);
	MachineCollisionComp->SetBoxExtent(FVector(32.0, 32.0, 32.0));
	MachineCollisionComp->SetRelativeLocation(FVector(0, 90, 0));
	MachineCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWashingMachine::MachineWashOnOverlap);

}


// Called when the game starts or when spawned
void AWashingMachine::BeginPlay()
{
	Super::BeginPlay();

	// Set references 
	ShopManagerRef = Cast<AShopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AShopManager::StaticClass()));
	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());

	// Upgrade Utilities Cost in ShopManager
	ShopManagerRef->UpdateUtilitiesCost(this->UpgradeState);

	// Bind Timer Function
	TimerDelegate.BindUFunction(this, "WashCycle");
}

// When Pawn is in range of this machine, allow them to purchase an upgrade during night cycle, 
// and use the machine during the day cycle
void AWashingMachine::MachineWashOnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ALaundryBag* Bag = Cast<ALaundryBag>(OtherActor);
	if (OtherActor != this && Bag != nullptr && GameInstanceRef->GameState == EGameState::DAY && !bWashing)
	{

		if (ChaosState == EChaosState::WORKING)
		{
			// Trigger Blueprint event (Anims, etc.)
			K2_StartWashCycle();

			bWashing = true;
			washTimeElapsed = 0;
			GetWorld()->GetTimerManager().SetTimer(WashTimer, TimerDelegate, timerRate, true);
			Bag->Destroy();
		}
		
	}	
}

// Start Wash Cycle for this machine
// On end, Update Economy and Reputation
void AWashingMachine::WashCycle()
{
	washTimeElapsed++;
	UE_LOG(LogTemp, Display, TEXT("Wash Time elapsed: %f"), washTimeElapsed);
	float Chaos = FMath::RandRange(0.f, 100.f);


	if (washTimeElapsed >= WashCycleTime && ChaosState == EChaosState::WORKING)
	{
		// Trigger Blueprint event (Anims, etc.)
		K2_EndWashCycle();
		
		GameInstanceRef->GetTimerManager().ClearTimer(WashTimer);
		bWashing = false;
		ShopManagerRef->Economy.Cash += WashReward;
		ShopManagerRef->Reputation.Reputation += ReputationIncrease;
	}
	else if (washTimeElapsed < WashCycleTime && ChaosState == EChaosState::BROKEN) {
		// Pause Timer and only resume when player fixes the machine
		K2_InitChaosEvent();
		GetWorld()->GetTimerManager().PauseTimer(WashTimer);
	}
	else if (GameInstanceRef->GameState == EGameState::NIGHT)
	{
		// Trigger Blueprint event (Anims, etc.)
		K2_EndWashCycle();

		GameInstanceRef->GetTimerManager().ClearTimer(WashTimer);
		bWashing = false;
	}

	if (Chaos < (ChoasChancePercent*100.f)) ChaosState = EChaosState::BROKEN;
	UE_LOG(LogTemp, Display, TEXT("Broken if Chaos: %f < %f"), Chaos, ChoasChancePercent*100.f);
}


// Upgrade Machine and subtract from Economy
void AWashingMachine::UpgradeMachine()
{
	if (ShopManagerRef->Economy.Cash >= UpgradeCost && ChaosState == EChaosState::WORKING)
	{
		UpgradeCost += UpgradeIncrease;
		// Update new upgrade cost and State
		switch (UpgradeState)
		{
			case (EMachineUpgrade::SMALL) :
				UE_LOG(LogTemp, Display, TEXT("Upgrading Machine: Small to medium"));
				ShopManagerRef->BuyItem(UpgradeCost); 
				UpgradeState = EMachineUpgrade::MEDIUM;
				ShopManagerRef->Economy.CurrentMachines.RemoveSingleSwap(EMachineUpgrade::SMALL);
				K2_UpgradeEvent();
				ReputationIncrease++;
				ShopManagerRef->UpdateUtilitiesCost(UpgradeState);
				ChoasChancePercent -= ChoasChancePercentDecrease;
				WashReward += WashReward_Increase;
				break;
			case (EMachineUpgrade::MEDIUM) :
				UE_LOG(LogTemp, Display, TEXT("Upgrading Machine: Medium to Large"));
				ShopManagerRef->BuyItem(UpgradeCost);
				UpgradeState = EMachineUpgrade::LARGE;
				ShopManagerRef->Economy.CurrentMachines.RemoveSingleSwap(EMachineUpgrade::MEDIUM);
				K2_UpgradeEvent();
				ReputationIncrease++;
				ShopManagerRef->UpdateUtilitiesCost(UpgradeState);
				ChoasChancePercent -= ChoasChancePercentDecrease;
				WashReward += WashReward_Increase;
				break;
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Not enough money to upgrade Machine"));
	}


}


// Machine Chaos state is set to BROKEN, when fixed, Player can continue the wash cycle 
void AWashingMachine::FixMachine()
{
	if (ChaosState == EChaosState::BROKEN && GameInstanceRef->GameState == EGameState::DAY)
	{
		ChaosState = EChaosState::WORKING;
		K2_EndChaosEvent();
		GetWorld()->GetTimerManager().UnPauseTimer(WashTimer);
	}
	else if (ChaosState == EChaosState::BROKEN && GameInstanceRef->GameState == EGameState::NIGHT)
	{
		ChaosState = EChaosState::WORKING;
		// Trigger Blueprint event (Anims, etc.)
		K2_EndWashCycle();
		K2_EndChaosEvent();
		GameInstanceRef->GetTimerManager().ClearTimer(WashTimer);
		bWashing = false;
	}
}


// Called every frame
void AWashingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

