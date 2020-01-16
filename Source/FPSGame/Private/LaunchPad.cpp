// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSCharacter.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	ArrowComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent); 
	
	BaseComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComp2"));
	BaseComp->SetupAttachment(RootComponent);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);

}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPad::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping Launchpad"));
	//APawn* MyPawn = Cast<APawn>(OtherActor);
	AFPSCharacter* MyChar = Cast<AFPSCharacter>(OtherActor);
	if (MyChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Char %s"), *LaunchVector.ToString());
	 	FRotator MyRot = GetActorRotation();
		FVector LocalVector = MyRot.RotateVector(LaunchVector);
		MyChar->LaunchCharacter(LocalVector, true, true);
		OnLaunch(OtherActor, OtherComp);
	}
	else if (OtherComp && OtherComp->IsAnySimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Simulating yes"));
		FRotator MyRot = GetActorRotation();
		FVector LocalVector = MyRot.RotateVector(LaunchVector);
		OtherComp->AddImpulse(LocalVector, "", true);
		OnLaunch(OtherActor, OtherComp);
	}
}