// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidActor.h"

// Sets default values
ABoidActor::ABoidActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    m_perception_radius = 100;
    m_perception_field_of_view = PI;
}

// Called when the game starts or when spawned
void ABoidActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABoidActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (m_neighbors.Num() > 0)
    {
        update_cohesion();
        update_separation();
        update_alignment();
    }
    
    update_bounds();
    update_transform(DeltaTime);
}

void ABoidActor::configure(float cohesion_intensity, 
                           float separation_intensity, 
                           float alignment_intensity, 
                           float max_applied_force, 
                           float max_flock_speed)
{
    m_cohesion_intensity= cohesion_intensity;
    m_separation_intensity= separation_intensity;
    m_alignment_intensity= alignment_intensity;
    m_max_applied_force= max_applied_force;
    m_max_flock_speed= max_flock_speed;
    m_velocity= GetActorForwardVector();
    m_acceleration= GetActorForwardVector();
}

void ABoidActor::update_neighbors(const TArray<ABoidActor*> &flock)
{
    m_neighbors.Empty();

    for (ABoidActor *boid : flock)
    {
        if (boid != this && FVector::Distance(boid->GetActorLocation(), GetActorLocation()) < m_perception_radius)
        {
            m_neighbors.Add(boid);
        }
    }
}

void ABoidActor::update_cohesion()
{
    FVector center_of_mass= FVector();
    for (ABoidActor *boid : m_neighbors)
    {
        center_of_mass= center_of_mass + boid->GetActorLocation();
    }

    center_of_mass = center_of_mass * (1/m_neighbors.Num());
    FVector target_velocity= center_of_mass - GetActorLocation();
    target_velocity= limit(target_velocity, m_max_flock_speed);
    FVector force= (target_velocity - m_velocity);
    force= limit(force, m_max_applied_force);
    force= force * m_cohesion_intensity;
    m_acceleration= m_acceleration + force;
}

void ABoidActor::update_separation()
{
    FVector target_velocity= FVector();
    for (ABoidActor *boid : m_neighbors)
    {
        FVector delta= (boid->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        target_velocity = target_velocity + delta;
    }

    target_velocity = target_velocity * (1 / m_neighbors.Num());
    target_velocity= limit(target_velocity, m_max_flock_speed);
    FVector force= (target_velocity - m_velocity);
    force= limit(force, m_max_applied_force);
    force= force * m_separation_intensity;
    m_acceleration= m_acceleration + force;
}

void ABoidActor::update_alignment()
{
    FVector target_velocity= FVector();
    for (ABoidActor *boid : m_neighbors)
    {
        target_velocity += boid->GetActorForwardVector();
    }

    target_velocity *= (1 / m_neighbors.Num());
    target_velocity= limit(target_velocity, m_max_flock_speed);
    FVector force= (target_velocity - m_velocity);
    force= limit(force, m_max_applied_force);
    force= force * m_alignment_intensity;
    m_acceleration= m_acceleration + force;
}

void ABoidActor::update_bounds()
{
    if (GetActorLocation().Size() > k_max_range_from_origin) // meaning we're more than 100 units from the origin
    {
        m_acceleration = -GetActorLocation();
    }
}

void ABoidActor::update_transform(float delta_time)
{
    m_velocity= m_velocity + (m_acceleration * delta_time);
    m_velocity= limit(m_velocity, m_max_flock_speed);
    FVector new_position= GetActorLocation() + m_velocity;

    SetActorLocationAndRotation(new_position, m_velocity.GetSafeNormal().ToOrientationQuat());

    if (GetActorLocation().Size() > k_max_range_from_origin) // meaning we're more than X units from the origin
    {
        DrawDebugPoint(GetWorld(), GetActorLocation(), 10.0, FColor::Red);
    }
    else
    {
        DrawDebugPoint(GetWorld(), GetActorLocation(), 10.0, FColor::Green);
    }
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (10 * m_velocity.GetSafeNormal()), FColor::Black, false, -1.f, 0, 1);
}

FVector ABoidActor::limit(FVector vector, float max_magnitude)
{
    FVector result= vector;
    if (vector.Size() > max_magnitude)
    {
        result= result.GetSafeNormal() * max_magnitude;
    }
    return result;
}