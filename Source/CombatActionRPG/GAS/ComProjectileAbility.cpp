#include "ComProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "AssetTypeCategories.h"
#include "ComBaseProjectile.h"
#include "ComCombatAttributeSet.h"
#include "ComProjectileMagnitude.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CombatActionRPG/Character/ComPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/MovieSceneBaseValueEvaluatorSystem.h"
#include "Tasks/GameplayTask_WaitDelay.h"

void UComProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		return;
	}

	AComPlayerCharacter* Character { CastChecked<AComPlayerCharacter>(GetAvatarActorFromActorInfo()) };

	// Spawn a projectile in the direction of the cursor click
	if (APlayerController* PlayerController { Cast<APlayerController>(Character->GetController()) } )
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

const FGameplayTagContainer& UComProjectileAbility::GetAssetTagsBP() const
{	
	return GetAssetTags();	
}

float UComProjectileAbility::GetDamage() const
{
	float Damage { 0.0f };
	UGameplayEffect* GameplayEffect { ProjectileGameplayEffect->GetDefaultObject<UGameplayEffect>() };

	// For every modifier
	for (const FGameplayModifierInfo& Mod : GameplayEffect->Modifiers)
	{
		// If the modifier is applied to the damage attribute
		if (Mod.Attribute == UComCombatAttributeSet::GetDamageAttribute())
		{
			FGameplayEffectContextHandle EffectHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			EffectHandle.SetAbility(this);	
			FGameplayEffectSpec EffectSpec(GameplayEffect, EffectHandle, 1.f);

			// Get modifier magnitude which is the ability damage
			if (UComProjectileMagnitude* ProjectileMagnitude = Mod.ModifierMagnitude.GetCustomMagnitudeCalculationClass()->GetDefaultObject<UComProjectileMagnitude>())
			{
				Damage = ProjectileMagnitude->CalculateBaseMagnitude_Implementation(EffectSpec);
			}
		}
	}

	return Damage;
}

void UComProjectileAbility::SpawnProjectile()
{
	AComPlayerCharacter* Character { CastChecked<AComPlayerCharacter>(GetAvatarActorFromActorInfo()) };

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

	AComBaseProjectile* Projectile { GetWorld()->SpawnActor<AComBaseProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation, SpawnParams) };
	Projectile->HitActorGameplayEffect = ProjectileGameplayEffect;
	Projectile->InstigatorAbility = this;
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
