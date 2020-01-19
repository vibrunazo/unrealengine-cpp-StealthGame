// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
//#include "Engine/EngineTypes.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"
#include "GameFramework/Controller.h"
#include "AIController.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateAbstractDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHear);
	OriginalRotation = GetActorRotation();
	
	GoToNextWaypoint();
	//UpdateDistanceToNext();
}

void AFPSAIGuard::GoToNextWaypoint()
{
	UE_LOG(LogTemp, Warning, TEXT("GoToWp: from %d to %d of %d"), CurrentWaypoint, NextWaypoint, Waypoints.Num());
	if (Waypoints.Num() <= NextWaypoint) { return; }
	AActor* Wp = Waypoints[NextWaypoint];
	if (Wp == nullptr) { return;  }
	AController* MyController = GetController();
	AAIController* MyAiCont = Cast<AAIController>(MyController);
	if (!IsValid(MyAiCont)) { return;  }
	MyAiCont->MoveToActor(Wp);
	//DistanceToNext = FVector::Dist(GetActorLocation(), W*/aypoints[CurrentWaypoint]->GetActorLocation());
	
}

void AFPSAIGuard::ArriveAtWaypoint()
{
	//UE_LOG(LogTemp, Warning, TEXT("Arrived: from %d to %d of %d"), CurrentWaypoint, NextWaypoint, Waypoints.Num());
	CurrentWaypoint = NextWaypoint;
	NextWaypoint++;
	if (NextWaypoint >= Waypoints.Num()) { NextWaypoint = 0; }
	OriginalRotation = GetActorRotation();
	GoToNextWaypoint();
	//UE_LOG(LogTemp, Warning, TEXT("After: from %d to %d of %d"), CurrentWaypoint, NextWaypoint, Waypoints.Num());
}

void AFPSAIGuard::PauseMove()
{
	AController* MyController = GetController();
	AAIController* MyAiCont = Cast<AAIController>(MyController);
	if (!IsValid(MyAiCont)) { return; }
	MyAiCont->MoveToActor(this);
}

void AFPSAIGuard::UpdateDistanceToNext()
{
	if (Waypoints.Num() == 0) { return;  }
	DistanceToNext = FVector::Dist(GetActorLocation(), Waypoints[NextWaypoint]->GetActorLocation());
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("I see %s"), *SeenPawn->GetName());
	if (SeenPawn == nullptr)
	{
		return;
	}
	SetGuardState(EAIState::Alerted);
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Purple, false, 10.0f);
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
		//UE_LOG(LogTemp, Warning, TEXT("GuardState: %d"), GuardState);
	}
}

void AFPSAIGuard::OnHear(APawn * InstigatorPawn, const FVector & Location, float volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

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
	SetGuardState(EAIState::Suspecious);
	PauseMove();
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
	GoToNextWaypoint();
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;

	OnStateChanged(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PawnSensingComp->OnSeePawn.GetAllObjects();
	UpdateDistanceToNext();
	//UE_LOG(LogTemp, Warning, TEXT("Distance to next WP: %f"), DistanceToNext);
	if (DistanceToNext < 100.0f && CurrentWaypoint != NextWaypoint)
	{
		ArriveAtWaypoint();
	}
	
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

