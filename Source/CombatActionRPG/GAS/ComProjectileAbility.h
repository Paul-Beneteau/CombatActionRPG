#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComProjectileAbility.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Make ability tags visible for blueprint
	UFUNCTION(BlueprintCallable, Category="Tag")
	const FGameplayTagContainer& GetAssetTagsBP() const;

	// Get the ability damage (the gameplay effect modifier magnitude)
	UFUNCTION(BlueprintCallable, Category="Damage")
	float GetDamage() const;
	
protected:
	// Projectile spawned
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	// Gameplay effect when actor is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> ProjectileGameplayEffect;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> Icon;
	
	UFUNCTION()
	void SpawnProjectile();
};
