#include "ComBaseProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "CombatActionRPG/CombatActionRPG.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "AbilitySystemInterface.h"
#include "CombatActionRPG/Character/ComPlayerCharacter.h"
#include "CombatActionRPG/Character/ComNonPlayerCharacter.h"

AComBaseProjectile::AComBaseProjectile()
{
	RootComp = CreateDefaultSubobject<USphereComponent>("RootComp");
	RootComponent = RootComp;
	RootComp->SetSphereRadius(0.0f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComp);
		
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1900.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void AComBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AComBaseProjectile::OnActorOverlap);
}

void AComBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HitWorldParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldParticleEffect, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(ComLog, Warning, TEXT("AComBaseProjectile: HitWorldEffect has not been set"));
	}

	SetLifeSpan(3.0f);
}

// Apply the gameplay effect to the actor hit
void AComBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator() || OtherActor == this)
	{
		return;
	}

	if (HitActorParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitActorParticleEffect, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(ComLog, Warning, TEXT("AComBaseProjectile: HitActorEffect has not been set"));
	}

	AComPlayerCharacter* PlayerCharacter { Cast<AComPlayerCharacter>(GetInstigator())};
	AComNonPlayerCharacter* NonPlayerCharacter { Cast<AComNonPlayerCharacter>(OtherActor)};
	
	if (PlayerCharacter && NonPlayerCharacter)
	{
		UAbilitySystemComponent* TargetAbilitySystemComp = NonPlayerCharacter->GetAbilitySystemComponent();
		check(TargetAbilitySystemComp);
		UAbilitySystemComponent* SourceAbilitySystemComp = PlayerCharacter->GetAbilitySystemComponent();
		check(SourceAbilitySystemComp);
	
		FGameplayEffectContextHandle EffectHandle = SourceAbilitySystemComp->MakeEffectContext();
		EffectHandle.SetAbility(InstigatorAbility);
		
		SourceAbilitySystemComp->ApplyGameplayEffectToTarget(HitActorGameplayEffect->GetDefaultObject<UGameplayEffect>(),
			TargetAbilitySystemComp, 1.0f, EffectHandle);
	}
	else
	{
		UE_LOG(ComLog, Error, TEXT("AComBaseProjectile: Can't find instigator or target"));
	}	
	
	Destroy();
}
