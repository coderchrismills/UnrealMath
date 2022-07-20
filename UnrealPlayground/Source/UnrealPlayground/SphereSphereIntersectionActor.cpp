// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereSphereIntersectionActor.h"

// Sets default values
ASphereSphereIntersectionActor::ASphereSphereIntersectionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
}

// Called when the game starts or when spawned
void ASphereSphereIntersectionActor::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> children;
	GetAttachedActors(children);

	for (AActor* actor : children)
	{
		FString name = actor->GetActorLabel();
		if (name == TEXT("SS_Sphere1"))
		{
			m_sphere_one = actor;
		}
		else if (name == TEXT("SS_Sphere2"))
		{
			m_sphere_two = actor;
		}
	}
}

// Called every frame
void ASphereSphereIntersectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_sphere_one == nullptr || m_sphere_two == nullptr)
	{
		return;
	}

	FVector sphere_one_center = m_sphere_one->GetActorLocation();
	FVector sphere_two_center = m_sphere_two->GetActorLocation();

	FVector s1_to_s2 = sphere_two_center - sphere_one_center;

	FVector orgin;
	FVector bounds_extent;

	m_sphere_one->GetActorBounds(false, orgin, bounds_extent);
	float sphere_one_radius = bounds_extent.X;

	m_sphere_two->GetActorBounds(false, orgin, bounds_extent);
	float sphere_two_radius = bounds_extent.X;

	float sum_sphere_radii = sphere_one_radius + sphere_two_radius;

	float s1_to_s2_legth = s1_to_s2.Size();

	float minimum_radius = FMath::Min(sphere_one_radius, sphere_two_radius);

	DrawDebugLine(GetWorld(), sphere_one_center, sphere_two_center, FColor::Emerald, false, -1, 0, 10);

	if (s1_to_s2_legth < sum_sphere_radii && s1_to_s2_legth > minimum_radius)
	{
		
		float d = s1_to_s2_legth;
		float s1_to_p_length = (d * d - sphere_two_radius * sphere_two_radius + sphere_one_radius * sphere_one_radius);
		FVector point_p = sphere_one_center + ((s1_to_s2 / s1_to_s2_legth) * (s1_to_p_length / (2 * d)));
		
		DrawDebugLine(GetWorld(), sphere_one_center, point_p, FColor::Purple, false, -1, 0, 20);

		FVector s1_to_p = point_p - sphere_one_center;
		float overlap_radius = FMath::Sqrt((sphere_one_radius * sphere_one_radius) - (s1_to_p.Size() * s1_to_p.Size()));

		FVector s1_to_p_normalized = s1_to_p;
		s1_to_p_normalized.Normalize();

		FVector other_vector = s1_to_p + FVector(overlap_radius, overlap_radius, overlap_radius);
		other_vector.Normalize();

		FVector s1_to_p_orthoganal1 = FVector::CrossProduct(s1_to_p_normalized, other_vector);
		s1_to_p_orthoganal1.Normalize();

		FVector s1_to_p_orthoganal2 = FVector::CrossProduct(s1_to_p, s1_to_p_orthoganal1);
		s1_to_p_orthoganal2.Normalize();

		DrawDebugCircle(GetWorld(), point_p, overlap_radius, 180, FColor::Orange, false, -1, 0, 15, s1_to_p_orthoganal2, s1_to_p_orthoganal1);
	}
}

