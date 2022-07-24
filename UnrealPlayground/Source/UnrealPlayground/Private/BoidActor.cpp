// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidActor.h"

// Sets default values
ABoidActor::ABoidActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    m_cohesion_intensity= 0;
    m_separation_intensity= 0;
    m_alignment_intensity= 0;
    m_max_applied_force= 0;
    m_max_flock_speed= 0;
    m_perception_radius= 0;
    m_perception_radius= 0;
    m_velocity= FVector();
    m_acceleration= FVector();
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

    update_debug();
}

void ABoidActor::configure(float cohesion_intensity, 
                           float separation_intensity, 
                           float alignment_intensity,
                           float perception_radius,
                           float max_applied_force, 
                           float max_flock_speed)
{
    m_cohesion_intensity= cohesion_intensity;
    m_separation_intensity= separation_intensity;
    m_alignment_intensity= alignment_intensity;
    m_max_applied_force= max_applied_force;
    m_max_flock_speed= max_flock_speed;
    m_perception_radius= perception_radius;
}

void ABoidActor::update_neighbors(const TArray<ABoidActor*> &flock)
{
    m_neighbors.Empty();

    bool has_drawn = false;
    for (ABoidActor *boid : flock)
    {
        FVector delta= (boid->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        float n_dot_v = FVector::DotProduct(delta, GetActorForwardVector());
        float angle= FMath::Acos(n_dot_v);

        if (boid != this && 
            FVector::Distance(boid->GetActorLocation(), GetActorLocation()) < m_perception_radius &&
            angle < PI/2)
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
    center_of_mass= center_of_mass / m_neighbors.Num();
    FVector target_velocity= center_of_mass - GetActorLocation();
    seek(target_velocity, m_cohesion_intensity);
}

void ABoidActor::update_separation()
{
    FVector target_velocity= FVector();
    for (ABoidActor *boid : m_neighbors)
    {
        FVector delta= (GetActorLocation() - boid->GetActorLocation());
        float distance= FVector::Distance(GetActorLocation(), boid->GetActorLocation());
        delta= delta / distance;
        target_velocity= target_velocity + delta;
    }

    target_velocity= target_velocity / m_neighbors.Num();
    seek(target_velocity, m_separation_intensity);
}

void ABoidActor::update_alignment()
{
    FVector target_velocity= FVector();
    for (ABoidActor *boid : m_neighbors)
    {
        target_velocity += boid->get_velocity();
    }

    target_velocity= target_velocity / m_neighbors.Num();
    seek(target_velocity, m_alignment_intensity);
}

void ABoidActor::update_bounds()
{
    FVector delta= GetActorLocation();
    float distance= k_max_range_from_origin - GetActorLocation().Size();
    distance= distance < 1.0f ? 1.0f : distance;
    float intensity= (1 / (distance * distance));
    FVector target_velocity= -GetActorLocation().GetSafeNormal() * intensity;
    m_acceleration= m_acceleration + target_velocity;
}

void ABoidActor::update_transform(float delta_time)
{
    m_velocity= m_acceleration;
    m_velocity= limit(m_velocity, m_max_flock_speed);

    FVector new_position= GetActorLocation() + m_velocity;
    FQuat new_orientation= m_velocity.Rotation().Quaternion();
    SetActorLocationAndRotation(new_position, new_orientation);

    if (GetActorLocation().Size() > k_max_range_from_origin)
    {
        DrawDebugPoint(GetWorld(), GetActorLocation(), 10.0, m_is_debug_enabled ? FColor::Green : FColor::Red);
    } 
    else
    {
        DrawDebugPoint(GetWorld(), GetActorLocation(), 10.0, m_is_debug_enabled ? FColor::Green : FColor::Blue);
    }
    DrawDebugCircleArc(GetWorld(), GetActorLocation(), 10.0, GetActorForwardVector(), PI/2, 36, FColor::Purple);
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (10 * GetActorForwardVector().GetSafeNormal()), FColor::Black, false, -1.f, 0, 1);
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

void ABoidActor::set_debug_enabled()
{
    m_is_debug_enabled = true;
}

void ABoidActor::update_debug()
{
    if (m_is_debug_enabled)
    {
        for (ABoidActor *boid : m_neighbors)
        {
            DrawDebugCircle(GetWorld(), boid->GetActorLocation(), 20, 36, FColor::Orange);
        }
    }
}

void ABoidActor::seek(FVector target_velocity, float intensity)
{
    target_velocity= target_velocity.GetSafeNormal() * m_max_flock_speed;
    FVector force= (target_velocity - m_velocity);
    force= limit(force, m_max_applied_force);
    force= force * intensity;
    m_acceleration= m_acceleration + force;
}