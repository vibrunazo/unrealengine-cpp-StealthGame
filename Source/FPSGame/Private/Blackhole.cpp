// Fill out your copyright notice in the Description page of Project Settings.


#include "Blackhole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "UObject/Class.h"

// Sets default values
ABlackhole::ABlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	 MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp = CreateDefaultObject<UStaticMeshComponent>(TEXT("MeshComp"));
	 RadialComp = CreateAbstractDefaultSubobject<URadialForceComponent>(TEXT("RadialComp"));
	 RootComponent = MeshComp;
	 RadialComp->SetupAttachment(MeshComp);
	 RadialComp->Radius = 1600;
	 RadialComp->ImpulseStrength = -50;
	 RadialComp->bImpulseVelChange = true;
	 MeshComp->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Your message"));
}

// Called every frame
void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//RadialComp->FireImpulse();

}

void ABlackhole::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Other->Destroy();
}


