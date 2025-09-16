#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ComProjectileMagnitude.generated.h"

UCLASS()
class COMBATACTIONRPG_API UComProjectileMagnitude : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UComProjectileMagnitude();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	// Capture MaxHealth source attribute
	FGameplayEffectAttributeCaptureDefinition SourceBaseDamageDef;
};
