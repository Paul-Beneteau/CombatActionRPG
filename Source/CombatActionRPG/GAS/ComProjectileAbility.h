#pragma once

#include "CoreMinimal.h"
#include "ComGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "ComProjectileAbility.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComProjectileAbility : public UComGameplayAbility
{
	GENERATED_BODY()

public:
	// Send a projectile toward the player attack click direction
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// Projectile class spawned
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> ProjectileClass;

	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void SpawnProjectile();
};
