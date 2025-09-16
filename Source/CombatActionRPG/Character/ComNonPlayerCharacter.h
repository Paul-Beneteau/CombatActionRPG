#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ComNonPlayerCharacter.generated.h"

class UComCombatAttributeSet;

UCLASS()
class COMBATACTIONRPG_API AComNonPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AComNonPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS")
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
};
