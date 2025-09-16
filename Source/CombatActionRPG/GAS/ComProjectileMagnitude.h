#pragma once

#include "CoreMinimal.h"
#include "ComDamageModifierAttributeSet.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComProjectileMagnitude.generated.h"

// TODO: move the struct in a seperate file
// Used by a data table to store every damage type (fire, lightning, projectile, attack, spell etc )
USTRUCT()
struct FComDamageData : public FTableRowBase
{
	GENERATED_BODY()

	// Optional tag that can be required to do damage (e.g. Increased fire damage attribute need an ability with fire
	// damage type tag, More damage doesn't need a required tag as it is applied for every abilities)
	UPROPERTY(EditDefaultsOnly)
	TOptional<FGameplayTag> RequiredTag;

	// Attribute from UComDamageModifierAttributeSet representing damage modifier (e.g. increased cold damage)
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute DamageModifierAttribute;
};

UCLASS()
class COMBATACTIONRPG_API UComProjectileMagnitude : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	// Contains every additive damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> AdditiveDamageModifierDataTable;

	// Contains every multiplicative damage modifier
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UDataTable> MultiplicativeDamageModifierDataTable;
	
};
