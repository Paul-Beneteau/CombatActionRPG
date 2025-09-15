#include "ComBaseProjectile.h"

#include "CombatActionRPG/CombatActionRPG.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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

	SetLifeSpan(3.0f);
}

void AComBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HitWorldEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitWorldEffect, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(ComLog, Warning, TEXT("AComBaseProjectile: HitWorldEffect has not been set"));
	}
}

void AComBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator() || OtherActor == this)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("AComBaseProjectile::OnActorOverlap"));

	if (HitActorEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitActorEffect, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(ComLog, Warning, TEXT("AComBaseProjectile: HitActorEffect has not been set"));
	}
	
	Destroy();
}
