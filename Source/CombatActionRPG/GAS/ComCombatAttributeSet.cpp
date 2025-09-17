#include "ComCombatAttributeSet.h"
#include "GameplayEffectExtension.h"

// TODO: Move initial values in data asset or initial gameplay effect
UComCombatAttributeSet::UComCombatAttributeSet() :
	MaxHealth(200),
	MaxMana(100),
	BaseDamage(20)
{
	InitHealth(GetMaxHealth());
	InitMana(GetMaxMana());
}

void UComCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Health value must be between 0 and MaxHealth
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// MaxHealth value can't be below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	if (Attribute == GetManaAttribute())
	{
		// Mana value must be between 0 and MaxMana
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		// MaxMana value can't be below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

bool UComCombatAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	HealthBeforeChange = GetHealth();
	ManaBeforeChange = GetMana();
	
	return true;
}

void UComCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator { EffectContext.GetOriginalInstigator() };

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp<int32>(FMath::RoundToInt(GetHealth()), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Rounding to int so there isn't a float damage value
		SetHealth(FMath::Clamp<int32>(FMath::RoundToInt(GetHealth() - GetDamage()), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}	
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		SetHealth(FMath::Clamp<int32>(FMath::RoundToInt(GetHealth() + GetHeal()), 0.0f, GetMaxHealth()));
		SetHeal(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{		
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	
	if (GetHealth() != HealthBeforeChange)
	{
		OnHealthChanged.Broadcast(Instigator, HealthBeforeChange, GetHealth());
	}
	else if (GetMana() != ManaBeforeChange)
	{
		OnManaChanged.Broadcast(Instigator, ManaBeforeChange, GetMana());
	}
	else if (Data.EvaluatedData.Attribute == GetBaseDamageAttribute())
	{		
		SetBaseDamage(GetBaseDamage());
	}
}