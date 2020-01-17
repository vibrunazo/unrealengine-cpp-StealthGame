// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
//#include "Engine/EngineTypes.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateAbstractDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHear);
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("I see %s"), *SeenPawn->GetName());
	if (SeenPawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Purple, false, 10.0f);
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
}

void AFPSAIGuard::OnHear(APawn * InstigatorPawn, const FVector & Location, float volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	//FVector LookLocation = Location;
	//LookLocation.Z = GetActorLocation().Z;
	//FVector Direction = LookLocation - GetActorLocation();
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 4.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PawnSensingComp->OnSeePawn.GetAllObjects();
	
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

