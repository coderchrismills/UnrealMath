// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidActor.h"
#include "BoidControllerActor.generated.h"

UCLASS()
class UNREALPLAYGROUND_API ABoidControllerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidControllerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TSubclassOf<AActor> m_actor_to_spawn;

	UFUNCTION()
	ABoidActor* spawn_boid(FVector location, FRotator rotation);

private:
	TArray<ABoidActor*> m_boids;
};
