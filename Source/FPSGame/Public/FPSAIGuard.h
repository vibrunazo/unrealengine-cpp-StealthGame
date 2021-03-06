// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspecious,
	Alerted
};
UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<AActor*> Waypoints;
	int CurrentWaypoint = -1;
	int NextWaypoint = 0;
	float DistanceToNext = 0.0f;
	void ArriveAtWaypoint();

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
	void OnHear(APawn* InstigatorPawn, const FVector& Location, float volume);

	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;
	
	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GoToNextWaypoint();
	void UpdateDistanceToNext();
	void PauseMove();

};
