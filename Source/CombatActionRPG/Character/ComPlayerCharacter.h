#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ComPlayerDataAsset.h"
#include "ComPlayerCharacter.generated.h"

class UGameplayEffect;
class UComProjectileAbility;
struct FEnhancedInputActionEventBinding;
class UComAbilitySystemComponent;
class UComDamageModifierAttributeSet;
class UComCombatAttributeSet;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class COMBATACTIONRPG_API AComPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	AComPlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> Ability);

	// Remove the current ability bound to the input action and binds the new ability
	UFUNCTION(BlueprintCallable)
	void SetInputActionAbility(UInputAction* InputAction, TSubclassOf<UGameplayAbility> Ability);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;

	/** Time Threshold to know if it was a  short click to set destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ClickToDestinationThreshold { 0.3f };	
	float SetDestinationTriggerDuration { 0.0f };

	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	UComAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComCombatAttributeSet> CombatAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GAS")
	TObjectPtr<UComDamageModifierAttributeSet> DamageAttributeSet;
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	TObjectPtr<UComPlayerDataAsset> PlayerData;

	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, uint32> InputHandleMap;
	
	// Saves input action handles to dynamically modify input ability binding.
	UPROPERTY(BlueprintReadOnly)
	TMap<TObjectPtr<UInputAction>, TSubclassOf<UGameplayAbility>> InputAbilityMap;
	
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
};
