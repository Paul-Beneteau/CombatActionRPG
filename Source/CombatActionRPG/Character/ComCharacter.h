#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "ComPlayerDataAsset.h"
#include "ComCharacter.generated.h"

class UComAbilitySystemComponent;
class UComDamageModifierAttributeSet;
class UComCombatAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class COMBATACTIONRPG_API AComCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	AComCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> Ability);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;

	/** Time Threshold to know if it was a  short click to set destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ClickToDestinationThreshold { 0.3f };	
	float SetDestinationTriggerDuration { 0.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	UComAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComDamageModifierAttributeSet> DamageAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UComPlayerDataAsset> PlayerData;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
};
