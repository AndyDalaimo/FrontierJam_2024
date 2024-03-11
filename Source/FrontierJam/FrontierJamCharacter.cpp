// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrontierJamCharacter.h"
#include "FrontierJamProjectile.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AFrontierJamCharacter

AFrontierJamCharacter::AFrontierJamCharacter()
{

	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AFrontierJamCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GameInstanceRef = Cast<UShopDayCycle>(GetWorld()->GetGameInstance());
	ShopManagerRef = Cast<AShopManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AShopManager::StaticClass()));

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AFrontierJamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrontierJamCharacter::Move);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AFrontierJamCharacter::Interact);


		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrontierJamCharacter::Look);
	}
}


void AFrontierJamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFrontierJamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFrontierJamCharacter::Interact(const FInputActionValue& Value)
{
	FVector Start = GetActorLocation() + FVector(0,0,50);
	FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 300.f;
	FString SpawnString = "SpawnMesh";

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,
		ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams()) && HitResult.GetActor()->IsValidLowLevel())
	{
		for (UActorComponent* comp : HitResult.GetActor()->GetComponents())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Interact: %s"), *comp->GetName());
			if (*comp->GetName() == SpawnString)
			{
				if (GameInstanceRef->GameState == EGameState::NIGHT) SpawnNewMachine((ASpawnManager*)HitResult.GetActor());
			}
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, .5f);
}


// Spawn New machine if Player has enough funds and is on Night Cycle
void AFrontierJamCharacter::SpawnNewMachine(ASpawnManager* SpawnManager)
{
	if (ShopManagerRef->Economy.Cash >= 150)
	{
		UE_LOG(LogTemp, Display, TEXT("Attempting to spawn Machine"));
		SpawnManager->SpawnNewMachine();
		SpawnManager->Destroy();
		ShopManagerRef->BuyItem(150.f);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Not Enough CASH"));
	}

}

void AFrontierJamCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFrontierJamCharacter::GetHasRifle()
{
	return bHasRifle;
}