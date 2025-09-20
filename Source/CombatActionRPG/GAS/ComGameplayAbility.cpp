#include "ComGameplayAbility.h"

#include "ComAbilityDamageCalculation.h"
#include "ComCombatAttributeSet.h"

UComGameplayAbility::UComGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

const FGameplayTagContainer& UComGameplayAbility::GetAssetTagsBP() const
{	
	return GetAssetTags();	
}

float UComGameplayAbility::GetDamage() const
{
	float Damage { 0.0f };
	UGameplayEffect* GameplayEffect { GameplayEffectClass->GetDefaultObject<UGameplayEffect>() };

	// For every modifier
	for (const FGameplayModifierInfo& Mod : GameplayEffect->Modifiers)
	{
		// If the modifier is applied to the damage attribute
		if (Mod.Attribute == UComCombatAttributeSet::GetDamageAttribute())
		{			
			FGameplayEffectContextHandle EffectHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			EffectHandle.SetAbility(this);	
			FGameplayEffectSpec EffectSpec(GameplayEffect, EffectHandle, 1.f);

			// Get modifier magnitude which is the ability damage
			if (UComAbilityDamageCalculation* AbilityDamage = Mod.ModifierMagnitude.GetCustomMagnitudeCalculationClass()->GetDefaultObject<UComAbilityDamageCalculation>())
			{
				Damage = AbilityDamage->CalculateBaseMagnitude_Implementation(EffectSpec);
			}
		}
	}

	return Damage;
}
