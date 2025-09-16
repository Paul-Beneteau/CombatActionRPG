#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ComDamageModifierAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class COMBATACTIONRPG_API UComDamageModifierAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedProjectileDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedFireDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedLightningDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedColdDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, IncreasedAttackDamage)
	ATTRIBUTE_ACCESSORS(UComDamageModifierAttributeSet, MoreDamage)
	
	UComDamageModifierAttributeSet();
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
protected:
	// Additive damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedProjectileDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedFireDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedLightningDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedColdDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData IncreasedAttackDamage;
	
	// Multiplicative damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoreDamage;
};
