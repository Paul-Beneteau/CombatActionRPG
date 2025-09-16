#include "ComNonPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "CombatActionRPG/GAS/ComCombatAttributeSet.h"

AComNonPlayerCharacter::AComNonPlayerCharacter()
{
	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	CombatAttributeSet = CreateDefaultSubobject<UComCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComCombatAttributeSet>(CombatAttributeSet);
}

void AComNonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystemComp->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AComNonPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp; 
}

