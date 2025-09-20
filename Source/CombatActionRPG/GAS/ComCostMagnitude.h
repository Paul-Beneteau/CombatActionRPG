#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComCostMagnitude.generated.h"

// Compute the 
UCLASS()
class COMBATACTIONRPG_API UComCostMagnitude : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
