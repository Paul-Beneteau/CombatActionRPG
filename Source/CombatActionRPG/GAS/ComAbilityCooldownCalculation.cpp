#include "ComAbilityCooldownCalculation.h"

#include "AbilitySystemInterface.h"
#include "ComCombatAttributeSet.h"
#include "ComGameplayAbility.h"
#include "CombatActionRPG/CombatActionRPG.h"

float UComAbilityCooldownCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Cooldown { 0.0f };
	
	IAbilitySystemInterface* Instigator = Cast<IAbilitySystemInterface>(Spec.GetEffectContext().GetInstigator());
	if (Instigator == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComAbilityDamageCalculation: Can't find gameplay effect instigator"));
		return 0.0f;
	}
	
	UAbilitySystemComponent* AbilitySystemComp = Instigator->GetAbilitySystemComponent();
	if (AbilitySystemComp == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComAbilityDamageCalculation: Can't find ability system component of instigator"));
		return 0.0f;
	}
		
	if (AbilitySystemComp->HasAttributeSetForAttribute(UComCombatAttributeSet::GetAttackSpeedAttribute()))
	{			
		if (float AttackSpeed { AbilitySystemComp->GetNumericAttribute(UComCombatAttributeSet::GetAttackSpeedAttribute()) })
		{
			// Attack speed is the number of attack per seconds. The cooldown in second is one second divided by the number of attack per seconds
			Cooldown = 1 / AttackSpeed;
		}
	}

	return Cooldown;
}
