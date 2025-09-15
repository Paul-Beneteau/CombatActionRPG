#include "ComCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "AbilitySystemComponent.h"
#include "CombatActionRPG/CombatActionRPG.h"

AComCharacter::AComCharacter()
{	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Don't want arm to rotate when character does
	SpringArmComp->SetUsingAbsoluteRotation(true); 
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
	// Don't want to pull camera in when it collides with level
	SpringArmComp->bDoCollisionTest = false;

	// Camera does not rotate relative to arm
	CameraComp->bUsePawnControlRotation = false; 
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
}

void AComCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (PlayerData == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("AComCharacter: Player data asset has not been set"));
		return;
	}
	
	const APlayerController* PlayerController { GetController<APlayerController>() };
	check(PlayerController);	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };
	check(LocalPlayer);	
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		InputSubsystem->AddMappingContext(PlayerData->DefaultInputContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind movement input
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Started, this, &AComCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Triggered, this, &AComCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Completed, this, &AComCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Canceled, this, &AComCharacter::OnSetDestinationReleased);

		// Bind Abilities input
		for (FComAbilityInput AbilityInput : PlayerData->InitialAbilities)
		{
			EnhancedInputComponent->BindAction(AbilityInput.InputAction, ETriggerEvent::Started, this, &AComCharacter::OnActivateAbilityStarted, AbilityInput.Ability);
		}		
	}
}

void AComCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize GAS
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	
	for (FComAbilityInput AbilityInput : PlayerData->InitialAbilities)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityInput.Ability));
	}
}

UAbilitySystemComponent* AComCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AComCharacter::OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> Ability)
{
	AbilitySystemComp->TryActivateAbilityByClass(Ability);
}

void AComCharacter::OnInputStarted()
{
	GetController()->StopMovement();	
}

// Move toward destination while input is triggered
void AComCharacter::OnSetDestinationTriggered()
{
	SetDestinationTriggerDuration += GetWorld()->GetDeltaSeconds();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Get destination location at the cursor position
		FHitResult Hit;
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
		{
			// Move towards destination
			FVector WorldDirection = (Hit.Location - GetActorLocation()).GetSafeNormal();
			AddMovementInput(WorldDirection, 1.0, false);
		}
	}
}

// Move to destination if this was a short click
void AComCharacter::OnSetDestinationReleased()
{
	// If this is a short click
	if (SetDestinationTriggerDuration <= ClickToDestinationThreshold)
	{
		// Get destination location at the cursor position 
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			FHitResult Hit;
			if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
			{
				// Move to destination
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Hit.Location);
			}
		}		
	}

	SetDestinationTriggerDuration = 0.f;
}

