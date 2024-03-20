// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrench.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
AWrench::AWrench()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>WrenchMeshFinder(TEXT("/Game/AshStuff/FBX_Files/wrench"));
	WrenchMeshAsset = WrenchMeshFinder.Object;

	WrenchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WrenchMesh"));
	WrenchMesh->SetStaticMesh(WrenchMeshAsset);
	// WrenchMesh->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
	WrenchMesh->SetSimulatePhysics(true);
	WrenchMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	RootComponent = WrenchMesh;

}

void AWrench::DropWrench()
{
	this->DetachRootComponentFromParent(true);
	WrenchMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWrench::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWrench::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

