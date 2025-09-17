#include "ComProjectileMagnitude.h"

#include "AbilitySystemInterface.h"
#include "ComCombatAttributeSet.h"
#include "ComDamageModifierAttributeSet.h"
#include "CombatActionRPG/CombatActionRPG.h"
#include "CombatActionRPG/ComDataTableRow.h"

float UComProjectileMagnitude::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	IAbilitySystemInterface* Instigator = Cast<IAbilitySystemInterface>(Spec.GetEffectContext().GetInstigator());
	if (Instigator == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find gameplay effect instigator"));
		return 0.0f;
	}
	
	UAbilitySystemComponent* AbilitySystemComp = Instigator->GetAbilitySystemComponent();
	if (AbilitySystemComp == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find ability system component of instigator"));
		return 0.0f;
	}
	
	const UComDamageModifierAttributeSet* DamageModifierSet = Cast<UComDamageModifierAttributeSet>(AbilitySystemComp->GetAttributeSet(UComDamageModifierAttributeSet::StaticClass()));
	if (DamageModifierSet == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find damage attribute set of instigator"));
		return 0.0f;
	}	

	const UComCombatAttributeSet* CombatSet = Cast<UComCombatAttributeSet>(AbilitySystemComp->GetAttributeSet(UComCombatAttributeSet::StaticClass()));
	if (CombatSet == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find combat attribute set of instigator"));
		return 0.0f;
	}

	const UGameplayAbility* InstigatorAbility { Spec.GetEffectContext().GetAbility() };
	
	if (DamageModifierSet == nullptr)
	{
		UE_LOG(ComLog, Error, TEXT("UComProjectileMagnitude: Can't find gameplay ability instigator"));
		return 0.0f;
	}

	// Damage percent added additively to the total damage 
	float AdditiveDamageModifier { 1 };

	TArray<FComDamageModifierRow*> AdditiveDamageModifierRows;
	AdditiveDamageModifierDataTable->GetAllRows<FComDamageModifierRow>(FString(""), AdditiveDamageModifierRows);
	
	for (FComDamageModifierRow* AdditiveDamageModifierRow : AdditiveDamageModifierRows)
	{
		FGameplayTag RequiredTag = AdditiveDamageModifierRow->RequiredTag.Get(FGameplayTag::EmptyTag);
		
		// If there is no required tag or the instigator ability has the required tag 
		if (RequiredTag == FGameplayTag::EmptyTag || (InstigatorAbility->AbilityTags.HasTag(RequiredTag)))
		{
			// Add the additive damage attribute from the instigator attribute set
			AdditiveDamageModifier += AdditiveDamageModifierRow->DamageModifierAttribute.GetNumericValueChecked(DamageModifierSet) / 100;
		}
	}

	TArray<FComDamageModifierRow*> MultiplicativeDamageModifierRows;
	MultiplicativeDamageModifierDataTable->GetAllRows<FComDamageModifierRow>(FString(""), MultiplicativeDamageModifierRows);
	
	// Damage percent added multiplicatively to the total damage
	float MultiplicativeDamageModifier { 1 };

	// Iterates over every multiplicative damage type 
	for (FComDamageModifierRow* MultiplicativeDamageRow : MultiplicativeDamageModifierRows)
	{
		FGameplayTag RequiredTag = MultiplicativeDamageRow->RequiredTag.Get(FGameplayTag::EmptyTag);

		// If there is no required tag or the instigator ability has the required tag 
		if (RequiredTag == FGameplayTag::EmptyTag || (InstigatorAbility->AbilityTags.HasTag(RequiredTag)))			
		{
			// Add the multiplicative damage attribute from the instigator attribute set
			MultiplicativeDamageModifier *= (1 + (MultiplicativeDamageRow->DamageModifierAttribute.GetNumericValueChecked(DamageModifierSet) / 100));
		}
	}
	
	float Damage = { CombatSet->GetBaseDamage() * AdditiveDamageModifier * MultiplicativeDamageModifier };
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("UComProjectileMagnitude - Damage: %f"), Damage));
	
	return Damage;
}
