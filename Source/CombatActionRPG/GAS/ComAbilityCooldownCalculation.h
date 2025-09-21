#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComAbilityCooldownCalculation.generated.h"

// Compute the cooldown of an ability
UCLASS()
class COMBATACTIONRPG_API UComAbilityCooldownCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
