#include "ComPlayerCharacter.h"

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
#include "CombatActionRPG/GAS/ComAbilitySystemComponent.h"
#include "CombatActionRPG/GAS/ComCombatAttributeSet.h"
#include "CombatActionRPG/GAS/ComDamageModifierAttributeSet.h"

AComPlayerCharacter::AComPlayerCharacter()
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
	AbilitySystemComp = CreateDefaultSubobject<UComAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	CombatAttributeSet = CreateDefaultSubobject<UComCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComCombatAttributeSet>(CombatAttributeSet);
	DamageAttributeSet = CreateDefaultSubobject<UComDamageModifierAttributeSet>(TEXT("DamageAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComDamageModifierAttributeSet>(DamageAttributeSet);
}

void AComPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (PlayerData == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("AComPlayerCharacter: Player data asset has not been set"));
		return;
	}
	
	const APlayerController* PlayerController { GetController<APlayerController>() };
	check(PlayerController);	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };
	check(LocalPlayer);	
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem { LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() })
	{
		InputSubsystem->AddMappingContext(PlayerData->DefaultInputContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent { Cast<UEnhancedInputComponent>(InputComponent) })
	{
		// Bind movement input
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Started, this, &AComPlayerCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Triggered, this, &AComPlayerCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Completed, this, &AComPlayerCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(PlayerData->Move, ETriggerEvent::Canceled, this, &AComPlayerCharacter::OnSetDestinationReleased);

		// Bind Abilities input. Saves handle to modify dynamically input ability binding
		for (FComAbilityInput AbilityInput : PlayerData->InitialAbilities)
		{			
			if (UInputAction* InputAction = AbilityInput.InputAction.Get(nullptr))
			{
				uint32 Handle { EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started,
					this, &AComPlayerCharacter::OnActivateAbilityStarted, AbilityInput.Ability).GetHandle() };
			
				InputHandleMap.Add(InputAction, Handle);				
				InputAbilityMap.Add(InputAction, AbilityInput.Ability);
			}			
		}		
	}
}

void AComPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize GAS
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	
	for (FComAbilityInput AbilityInput : PlayerData->InitialAbilities)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityInput.Ability));
	}
}

UAbilitySystemComponent* AComPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AComPlayerCharacter::OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> Ability)
{
	AbilitySystemComp->TryActivateAbilityByClass(Ability);
}

void AComPlayerCharacter::SetInputActionAbility(UInputAction* InputAction, TSubclassOf<UGameplayAbility> Ability)
{
	check(InputAction && Ability);
	
	if (UEnhancedInputComponent* EnhancedInputComponent { Cast<UEnhancedInputComponent>(InputComponent) })
	{
		// Remove previous binding
		EnhancedInputComponent->RemoveBindingByHandle(*InputHandleMap.Find(InputAction));
		
		InputHandleMap.Remove(InputAction);
		
		// Add new binding and save the handle
		uint32 NewHandle { EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started,
	this, &AComPlayerCharacter::OnActivateAbilityStarted, Ability).GetHandle() };

		InputHandleMap.Remove(InputAction);
		InputHandleMap.Add(InputAction, NewHandle);

		InputAbilityMap.Remove(InputAction);
		InputAbilityMap.Add(InputAction, Ability);
	}
}

void AComPlayerCharacter::OnInputStarted()
{
	GetController()->StopMovement();	
}

// Move toward destination while input is triggered
void AComPlayerCharacter::OnSetDestinationTriggered()
{
	SetDestinationTriggerDuration += GetWorld()->GetDeltaSeconds();

	if (APlayerController* PlayerController { Cast<APlayerController>(GetController()) })
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
void AComPlayerCharacter::OnSetDestinationReleased()
{
	// If this is a short click
	if (SetDestinationTriggerDuration <= ClickToDestinationThreshold)
	{
		// Get destination location at the cursor position 
		if (APlayerController* PlayerController { Cast<APlayerController>(GetController()) })
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

