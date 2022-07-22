// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidActor.generated.h"

UCLASS()
class UNREALPLAYGROUND_API ABoidActor : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    ABoidActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    void configure(float cohesion_intensity, 
                   float separation_intensity, 
                   float alignment_intensity, 
                   float max_applied_force, 
                   float max_flock_speed);

    void update_neighbors(const TArray<ABoidActor*>& flock);

private:
    float m_cohesion_intensity;
    float m_separation_intensity;
    float m_alignment_intensity;
    float m_max_applied_force;
    float m_max_flock_speed;
    float m_perception_radius;
    float m_perception_field_of_view;

    const float k_max_range_from_origin= 200.f * 200.f;

    FVector m_velocity;
    FVector m_acceleration;
    TArray<ABoidActor*> m_neighbors;

    void update_cohesion();
    void update_separation();
    void update_alignment();
    void update_bounds();
    void update_transform(float delta_time);
    FVector limit(FVector vector, float max_magnitude);
};
