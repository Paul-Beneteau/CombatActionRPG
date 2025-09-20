#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Containers/Map.h"
#include "ComPlayerDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;
class UGameplayAbility;

USTRUCT()
struct FComAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TOptional<TObjectPtr<UInputAction>> InputAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

UCLASS()
class COMBATACTIONRPG_API UComPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TArray<FComAbilityInput> InitialAbilities;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> Move;
};
