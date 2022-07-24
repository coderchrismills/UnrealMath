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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
	float m_cohesion_intensity;
	
	UFUNCTION(BlueprintCallable)
	void set_cohesion_intensity(float value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
    float m_separation_intensity;
    
	UFUNCTION(BlueprintCallable)
    void set_separation_intensity(float value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
    float m_alignment_intensity;
    
	UFUNCTION(BlueprintCallable)
    void set_alignment_intensity(float value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
    float m_radius_of_perception;

    UFUNCTION(BlueprintCallable)
    void set_perception_radius(float value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
    float m_max_applied_force;

    UFUNCTION(BlueprintCallable)
    void set_max_applied_force(float value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boid Config")
    float m_max_flock_speed;

    UFUNCTION(BlueprintCallable)
    void set_max_flock_speed(float value);

private:
	TArray<ABoidActor*> m_boids;
};
