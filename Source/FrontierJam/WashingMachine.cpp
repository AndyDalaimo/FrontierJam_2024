// Fill out your copyright notice in the Description page of Project Settings.


#include "WashingMachine.h"

#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

// Sets default values
AWashingMachine::AWashingMachine() : UpgradeCost(100), UpgradeIncrease(250), UpgradeState(EMachineUpgrade::SMALL), WashCycleTime(5), bWashing(false)
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
		bWashing = true;
		washTimeElapsed = 0;
		GetWorld()->GetTimerManager().SetTimer(WashTimer, TimerDelegate, timerRate, true);
		Bag->Destroy();
	}	
}

// Start Wash Cycle for this machine
// On end, Update Economy and Reputation
void AWashingMachine::WashCycle()
{
	washTimeElapsed++;
	UE_LOG(LogTemp, Display, TEXT("Wash Time elapsed: %f"), washTimeElapsed);

	if (washTimeElapsed >= WashCycleTime)
	{
		GameInstanceRef->GetTimerManager().ClearTimer(WashTimer);
		bWashing = false;
		ShopManagerRef->Economy.Cash += WashReward;
	}
}


// Upgrade Machine and subtract from Economy
void AWashingMachine::UpgradeMachine()
{
	if (ShopManagerRef->Economy.Cash >= UpgradeCost)
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
				ShopManagerRef->UpdateUtilitiesCost(UpgradeState);
				WashReward += WashReward_Increase;
				break;
			case (EMachineUpgrade::MEDIUM) :
				UE_LOG(LogTemp, Display, TEXT("Upgrading Machine: Medium to Large"));
				ShopManagerRef->BuyItem(UpgradeCost);
				UpgradeState = EMachineUpgrade::LARGE;
				ShopManagerRef->Economy.CurrentMachines.RemoveSingleSwap(EMachineUpgrade::MEDIUM);
				ShopManagerRef->UpdateUtilitiesCost(UpgradeState);
				WashReward += WashReward_Increase;
				break;
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Not enough money to upgrade Machine"));
	}


}


// Called every frame
void AWashingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

