#include "ComProjectileAbility.h"

#include "AssetTypeCategories.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CombatActionRPG/Character/ComCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tasks/GameplayTask_WaitDelay.h"

void UComProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ComProjectileAbility"));

	AComCharacter* Character = CastChecked<AComCharacter>(GetAvatarActorFromActorInfo());

	// Spawn a projectile in the direction of the cursor click
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		FHitResult Hit;
		
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit))
		{			
			// Clear bOrientRotationToMovement so that character can rotate. Best way to rotate would be to use
			// animation but this is not the focus of the project
			Character->GetCharacterMovement()->bOrientRotationToMovement = false;

			FRotator CharacterRotation { (Hit.Location - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation() };
			
			// Rotate toward click location
			PlayerController->SetControlRotation(CharacterRotation);

			// Spawn projectile. Loop to wait until character finish rotating
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UComProjectileAbility::SpawnProjectile, 0.1f, true);
		}
	}
}

void UComProjectileAbility::SpawnProjectile()
{
	AComCharacter* Character = CastChecked<AComCharacter>(GetAvatarActorFromActorInfo());

	// Wait until character finish rotating
	if (Character->ReachedDesiredRotation() == false)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	// Reactivate bOrientRotationToMovement after character rotated
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Spawn Projectile
	FVector ProjectileLocation = Character->GetMesh()->GetSocketLocation("BowEmitterSocket");		
	FRotator ProjectileRotation = Character->GetActorRotation();
	
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Character;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, ProjectileLocation, ProjectileRotation, SpawnParams);
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
		FString::Printf(TEXT("UComProjectileAbility::SpawnProjectile(): %s"), *GetNameSafe(Projectile)));

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
