#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComGameplayAbility.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UComGameplayAbility();

	// Make ability tags visible for blueprint
	UFUNCTION(BlueprintCallable, Category="Ability")
	const FGameplayTagContainer& GetAssetTagsBP() const;

	// Get the ability final damage (the gameplay effect modifier magnitude)
	UFUNCTION(BlueprintCallable, Category="Ability")
	float GetFinalDamage() const;

	int32 GetBaseDamage() const { return BaseDamage; }

	UFUNCTION(BlueprintCallable, Category="Ability")
	int32 GetCost() const { return Cost; }

protected:
	// Gameplay effect when actor is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> Icon;
};
