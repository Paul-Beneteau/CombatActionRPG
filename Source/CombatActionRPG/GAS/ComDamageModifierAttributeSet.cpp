#include "ComDamageModifierAttributeSet.h"

#include "GameplayEffectExtension.h"

UComDamageModifierAttributeSet::UComDamageModifierAttributeSet()
{
}

void UComDamageModifierAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Check that attribute value can't go below 0. We don't check which attribute it is because it applies to every attributes
	NewValue = FMath::Clamp<float>(NewValue, 0.0f, NewValue);
}

// Ensure value doesn't get below 0
void UComDamageModifierAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetAddedPhysicalDamageAttribute())
	{		
		SetAddedPhysicalDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetAddedPhysicalDamage()), 0.0f, GetAddedPhysicalDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetAddedFireDamageAttribute())
	{		
		SetAddedFireDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetAddedFireDamage()), 0.0f, GetAddedFireDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetAddedLightningDamageAttribute())
	{		
		SetAddedLightningDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetAddedLightningDamage()), 0.0f, GetAddedLightningDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetAddedColdDamageAttribute())
	{		
		SetAddedColdDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetAddedColdDamage()), 0.0f, GetAddedColdDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncreasedDamageAttribute())
	{		
		SetIncreasedDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedDamage()), 0.0f, GetIncreasedDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncreasedFireDamageAttribute())
	{		
		SetIncreasedFireDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedFireDamage()), 0.0f, GetIncreasedFireDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncreasedLightningDamageAttribute())
	{		
		SetIncreasedLightningDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedLightningDamage()), 0.0f, GetIncreasedLightningDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncreasedColdDamageAttribute())
	{		
		SetIncreasedColdDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedColdDamage()), 0.0f, GetIncreasedColdDamage()));
	}	
	else if (Data.EvaluatedData.Attribute == GetIncreasedAttackDamageAttribute())
	{		
		SetIncreasedAttackDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedAttackDamage()), 0.0f, GetIncreasedAttackDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncreasedProjectileDamageAttribute())
	{		
		SetIncreasedProjectileDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetIncreasedProjectileDamage()), 0.0f, GetIncreasedProjectileDamage()));
	}	
	else if (Data.EvaluatedData.Attribute == GetMoreDamageAttribute())
	{
		SetMoreDamage(FMath::Clamp<int32>(FMath::RoundToInt(GetMoreDamage()), 0.0f, GetMoreDamage()));
	}
}