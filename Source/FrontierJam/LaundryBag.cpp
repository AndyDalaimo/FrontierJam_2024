// Fill out your copyright notice in the Description page of Project Settings.


#include "LaundryBag.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
ALaundryBag::ALaundryBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LaundryMeshFinder(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule"));
	LaundryMeshAsset = LaundryMeshFinder.Object;

	LaundryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaundryMesh"));
	LaundryMesh->SetStaticMesh(LaundryMeshAsset);
	LaundryMesh->SetSimulatePhysics(true);
	RootComponent = LaundryMesh;

}

// Called when the game starts or when spawned
void ALaundryBag::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaundryBag::Launch(FVector LaunchDir)
{
	this->DetachRootComponentFromParent(true);
	LaundryMesh->SetSimulatePhysics(true);
	LaundryMesh->AddImpulse(LaunchDir);
	
}

// Called every frame
void ALaundryBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

