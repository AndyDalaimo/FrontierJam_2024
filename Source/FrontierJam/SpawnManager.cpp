// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnMesh"));
	SpawnMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	RootComponent = SpawnMesh;

	SpawnCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnCollider"));
	SpawnCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

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
	
}

// Allow Player to Purchase new machine during Night Cycle
void ASpawnManager::PlayerInteractionOnOverlap(UPrimitiveComponent* OverlappedComponent, 
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
		case (EGameState::DAY):
			UE_LOG(LogTemp, Display, TEXT("Player Interaction: Day Cycle"));
			break;
		case (EGameState::NIGHT):
			UE_LOG(LogTemp, Display, TEXT("Player Interaction: Night Cycle"));

			break;
		default:
			UE_LOG(LogTemp, Display, TEXT("Player Interaction: Default"));
		}
	}
}

// Player has bought a new Washing Machine. Spawn it at MachineSpawnLocation
void ASpawnManager::SpawnNewMachine()
{
	FActorSpawnParameters params;
	AWashingMachine* newMachine = Cast<AWashingMachine>(GetWorld()->SpawnActor<AWashingMachine>(MachineSpawnLocation->GetComponentLocation(), FRotator(0, 0, 0), params));
	UE_LOG(LogTemp, Warning, TEXT("Spawned New Machine"));
}


// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

