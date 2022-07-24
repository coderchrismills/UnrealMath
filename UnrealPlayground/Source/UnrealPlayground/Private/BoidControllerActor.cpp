// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidControllerActor.h"

// Sets default values
ABoidControllerActor::ABoidControllerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    m_cohesion_intensity= 0.1;
    m_separation_intensity= 0.2;
    m_alignment_intensity= 0.3;
    m_radius_of_perception= 50;
    m_max_applied_force= 0.1;
    m_max_flock_speed= 0.1;
}

// Called when the game starts or when spawned
void ABoidControllerActor::BeginPlay()
{
    Super::BeginPlay();

    for (int i= 0; i < 100; i++)
    {
        FVector random_location = FVector();
        random_location.X= FMath::FRandRange(-200.f, 200.f);
        random_location.Y= FMath::FRandRange(-200.f, 200.f);
        random_location.Z= FMath::FRandRange(-200.f, 200.f);

        FRotator random_rotation = FRotator();
        //random_rotation.Pitch= FMath::FRandRange(-180.f, 180.f);
        //random_rotation.Yaw= FMath::FRandRange(-180.f, 180.f);
        //random_rotation.Roll= FMath::FRandRange(-180.f, 180.f);

        ABoidActor *boid= spawn_boid(random_location, random_rotation);
        boid->configure(m_cohesion_intensity, 
                        m_separation_intensity, 
                        m_alignment_intensity, 
                        m_radius_of_perception, 
                        m_max_applied_force, 
                        m_max_flock_speed);

        if (i == 0)
        {
            boid->set_debug_enabled();
        }

        m_boids.Add(boid);
    }
}

void ABoidControllerActor::set_cohesion_intensity(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_cohesion_intensity(value);
    }
}

void ABoidControllerActor::set_separation_intensity(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_separation_intensity(value);
    }
}

void ABoidControllerActor::set_alignment_intensity(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_alignment_intensity(value);
    }
}

void ABoidControllerActor::set_perception_radius(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_perception_radius(value);
    }
}

void ABoidControllerActor::set_max_applied_force(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_max_applied_force(value);
    }
}

void ABoidControllerActor::set_max_flock_speed(float value)
{
    for (ABoidActor *boid : m_boids)
    {
        boid->set_max_flock_speed(value);
    }
}

// Called every frame
void ABoidControllerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    for (ABoidActor *boid : m_boids)
    {
        boid->update(m_boids, DeltaTime);
    }
}

ABoidActor* ABoidControllerActor::spawn_boid(FVector location, FRotator rotation)
{
    FActorSpawnParameters spawn_params;
    ABoidActor* spawned_boid_reference= GetWorld()->SpawnActor<ABoidActor>(m_actor_to_spawn, location, rotation, spawn_params);
    return spawned_boid_reference;
}

