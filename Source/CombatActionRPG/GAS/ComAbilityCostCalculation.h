#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComAbilityCostCalculation.generated.h"

// Compute the attribute cost of an ability
UCLASS()
class COMBATACTIONRPG_API UComAbilityCostCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
