#include "ComAbilityCostCalculation.h"

#include "ComProjectileAbility.h"
#include "CombatActionRPG/CombatActionRPG.h"

float UComAbilityCostCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UComProjectileAbility* ProjectileAbility { Cast<UComProjectileAbility>(Spec.GetEffectContext().GetAbility()) };
	
	if (ProjectileAbility == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find gameplay ability instigator"));
		return 0.0f;
	}

	// Return -Cost because we subtract instead of adding the cost to the attribute 
	return -ProjectileAbility->GetCost();
}
