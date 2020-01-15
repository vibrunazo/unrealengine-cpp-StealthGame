// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "PhysicsEngine/RadialForceComponent.h"
//#include "Components/SphereComponent.h"
#include "Blackhole.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API ABlackhole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackhole();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//URadialForceComponent* RadialComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Black Hole")
	float BHForce = 30000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Black Hole")
	float BHStrength = -5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Black Hole")
	bool bBHAccel = true;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

};
