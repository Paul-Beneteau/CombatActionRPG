#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComProjectileMagnitude.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComProjectileMagnitude : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	// Contains every additive damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> FlatDamageModifierTable;
	
	// Contains every additive damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> AdditiveDamageModifierTable;

	// Contains every multiplicative damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> MultiplicativeDamageModifierTable;	
};
