#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComBaseProjectile.generated.h"

class UGameplayEffect;
class USphereComponent;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class COMBATACTIONRPG_API AComBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AComBaseProjectile();
	
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USphereComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;

	// Effect when projectile is spawned
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> HitWorldParticleEffect;

	// Effect when projectile hit an actor
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> HitActorParticleEffect;

	// Gameplay effect when actor is hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> HitActorGameplayEffect;
	
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	
};
