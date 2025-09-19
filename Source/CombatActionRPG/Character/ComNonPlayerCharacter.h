#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ComNonPlayerCharacter.generated.h"

class UGameplayEffect;
class UComAbilitySystemComponent;
class UComCombatAttributeSet;

UCLASS()
class COMBATACTIONRPG_API AComNonPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AComNonPlayerCharacter();

	virtual void BeginPlay() override;

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	UComAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
};
