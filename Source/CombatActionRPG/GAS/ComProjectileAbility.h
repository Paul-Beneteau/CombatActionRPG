#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComProjectileAbility.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UComProjectileAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Make ability tags visible for blueprint
	UFUNCTION(BlueprintCallable, Category="Ability")
	const FGameplayTagContainer& GetAssetTagsBP() const;

	// Get the ability damage (the gameplay effect modifier magnitude)
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetDamage() const;

	int32 GetCost() const { return Cost; }
	
protected:
	// Projectile spawned
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> ProjectileClass;

	// Gameplay effect when actor is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> ProjectileGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> Icon;

	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void SpawnProjectile();
};
