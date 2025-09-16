#include "ComDamageModifierAttributeSet.h"

#include "GameplayEffectExtension.h"

// TODO: Move initial values in data asset or initial gameplay effect
UComDamageModifierAttributeSet::UComDamageModifierAttributeSet() :
	IncreasedFireDamage(10),
	IncreasedLightningDamage(10),
	IncreasedColdDamage(10)
{
}

void UComDamageModifierAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncreasedDamageAttribute())
	{		
		SetIncreasedDamage(GetIncreasedDamage());
	}
	
	if (Data.EvaluatedData.Attribute == GetIncreasedProjectileDamageAttribute())
	{		
		SetIncreasedProjectileDamage(GetIncreasedProjectileDamage());
	}

	if (Data.EvaluatedData.Attribute == GetIncreasedFireDamageAttribute())
	{		
		SetIncreasedFireDamage(GetIncreasedFireDamage());
	}

	if (Data.EvaluatedData.Attribute == GetIncreasedLightningDamageAttribute())
	{		
		SetIncreasedLightningDamage(GetIncreasedLightningDamage());
	}

	if (Data.EvaluatedData.Attribute == GetIncreasedColdDamageAttribute())
	{		
		SetIncreasedColdDamage(GetIncreasedColdDamage());
	}
	
	if (Data.EvaluatedData.Attribute == GetIncreasedAttackDamageAttribute())
	{		
		SetIncreasedColdDamage(GetIncreasedAttackDamage());
	}

	if (Data.EvaluatedData.Attribute == GetMoreDamageAttribute())
	{		
		SetMoreDamage(GetMoreDamage());
	}	
}