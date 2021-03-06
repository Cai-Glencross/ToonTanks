// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = StaticMesh;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Hit Comp is %s"), *HitComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor is %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherComp is %s"), *OtherComp->GetName());
	if (!GetOwner()) {
		Destroy();
		return;
	}
	if (OtherActor && OtherActor != this && OtherActor != GetOwner()) {
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			DamageAmount, 
			GetOwner()->GetInstigatorController(), 
			this, 
			UDamageType::StaticClass()
		);
		if (HitParticles) {
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}
		if (HitSound) {
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if (HitCameraShake) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
		}
		Destroy();
	} else if (OtherActor && OtherActor == GetOwner() && OtherComp && OtherComp->GetName().Equals(TEXT("Capsule Collider"))) {
		Destroy();
	}
}

