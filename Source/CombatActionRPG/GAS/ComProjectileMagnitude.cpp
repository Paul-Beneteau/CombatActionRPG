#include "ComProjectileMagnitude.h"

#include "ComCombatAttributeSet.h"

UComProjectileMagnitude::UComProjectileMagnitude()
{
	// Capture gameplay effect source instigator max health attribute
	SourceBaseDamageDef.AttributeToCapture = UComCombatAttributeSet::GetBaseDamageAttribute();
	SourceBaseDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SourceBaseDamageDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(SourceBaseDamageDef);
}

// Return source instigator max health as base magnitude
float UComProjectileMagnitude::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// retrieve instigator max health
	float SourceBaseDamage { 0.0f };
	
	GetCapturedAttributeMagnitude(SourceBaseDamageDef, Spec, EvaluationParameters, SourceBaseDamage);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("UComProjectileMagnitude - BaseDamage: %f"), SourceBaseDamage));
	
	return SourceBaseDamage;
}