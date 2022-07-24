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
                   float perception_radius,
                   float max_applied_force, 
                   float max_flock_speed);

    void update_neighbors(const TArray<ABoidActor*>& flock);

    void set_cohesion_intensity(float value) { m_cohesion_intensity= value; }
    void set_separation_intensity(float value) { m_separation_intensity= value; }
    void set_alignment_intensity(float value) { m_alignment_intensity= value; }
    void set_perception_radius(float value) { m_perception_radius= value; }
    void set_max_applied_force(float value) { m_max_applied_force= value; }
    void set_max_flock_speed(float value) { m_max_flock_speed= value; }
    void set_debug_enabled();
    FVector get_velocity() { return m_velocity; }

private:
    float m_cohesion_intensity;
    float m_separation_intensity;
    float m_alignment_intensity;
    float m_perception_radius;
    float m_max_applied_force;
    float m_max_flock_speed;
    bool m_is_debug_enabled; 

    const float k_max_range_from_origin= 600.0f;

    FVector m_velocity;
    FVector m_acceleration;
    TArray<ABoidActor*> m_neighbors;

    void update_cohesion();
    void update_separation();
    void update_alignment();
    void update_bounds();
    void update_transform(float delta_time);
    void update_debug();
    FVector limit(FVector vector, float max_magnitude);
    void seek(FVector target_velocity, float intensity);
};
