// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
ASpawnManager::ASpawnManager() : ItemToSpawn(EItemToSpawn::MACHINE), ReputationIncrease(0), SpawnRotation(FRotator(0,0,0)), ItemCost(200)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnMesh"));
	SpawnMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	RootComponent = SpawnMesh;

	MachineSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MachineSpawnLocation"));
	MachineSpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MachineSpawnLocation->SetWorldRotation(FRotator(90, 0, 0));
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// PlayerRef = Cast<AFrontierJamCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());
	ShopManagerRef = Cast<AShopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AShopManager::StaticClass()));
	
}

// Player has bought a new Washing Machine. Spawn it at MachineSpawnLocation
void ASpawnManager::SpawnNewMachine()
{
	FActorSpawnParameters params;
	// AWashingMachine* newMachine;
	AActor* BlueprintToSpawn;

	// Only let the player buy this item if they have enough cash 
	if (ShopManagerRef->Economy.Cash < ItemCost) return;

	switch (ItemToSpawn)
	{
		case (EItemToSpawn::MACHINE) :
			// newMachine = Cast<AWashingMachine>(GetWorld()->SpawnActor<AWashingMachine>(MachineSpawnLocation->GetComponentLocation(), FRotator(0, 0, 0), params));
			if (MachineBlueprintClass != nullptr) BlueprintToSpawn = GetWorld()->SpawnActor<AActor>(MachineBlueprintClass, MachineSpawnLocation->GetComponentLocation(), SpawnRotation, params);
			ShopManagerRef->BuyItem(ItemCost);
			this->Destroy();
			break;
		case (EItemToSpawn::DECOR) :
			if (DecorBlueprintClass != nullptr) BlueprintToSpawn = GetWorld()->SpawnActor<AActor>(DecorBlueprintClass, MachineSpawnLocation->GetComponentLocation(), SpawnRotation, params);
			ShopManagerRef->Reputation.Reputation += ReputationIncrease;
			ShopManagerRef->BuyItem(ItemCost);
			this->Destroy();
			break;
	}
	
}


// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

