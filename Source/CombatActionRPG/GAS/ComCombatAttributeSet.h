#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ComCombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, AActor*, EffectInstigator, float, OldValue, float, NewValue);
UCLASS()
class COMBATACTIONRPG_API UComCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, MaxHealth)	
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, HealthDamage)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Heal)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UComCombatAttributeSet, BaseDamage)
	
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	mutable FOnAttributeChanged OnManaChanged;

	UComCombatAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	
	float HealthBeforeChange { 0.0f };

	// Meta attribute that handle damage applied to the health
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData HealthDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Heal;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMana;
	
	float ManaBeforeChange { 0.0f };

	// Base attack damage of the owner 
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BaseDamage;
};
