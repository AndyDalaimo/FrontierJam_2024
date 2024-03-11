// Fill out your copyright notice in the Description page of Project Settings.


#include "WashingMachine.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
AWashingMachine::AWashingMachine() : UpgradeCost(100), UpgradeState(EMachineUpgrade::SMALL)
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
	MachineCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWashingMachine::PlayerInteractionOnOverlap);

}

// Called when the game starts or when spawned
void AWashingMachine::BeginPlay()
{
	Super::BeginPlay();

	// Set references 
	ShopManagerRef = Cast<AShopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AShopManager::StaticClass()));
	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());
}

// When Pawn is in range of this machine, allow them to purchase an upgrade during night cycle, 
// and use the machine during the day cycle
void AWashingMachine::PlayerInteractionOnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherComp)
	{
		switch (GameInstanceRef->GameState)
		{
			case (EGameState::DAY) :
				UE_LOG(LogTemp, Display, TEXT("Allow Player Interaction: Day Cycle"));
				break;
			case (EGameState::NIGHT) :
				UE_LOG(LogTemp, Display, TEXT("Allow Player Interaction: Night Cycle"));
				break;
			default :
				UE_LOG(LogTemp, Display, TEXT("Allow Player Interaction: Default"));
		}
	}	
}

// Called every frame
void AWashingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

