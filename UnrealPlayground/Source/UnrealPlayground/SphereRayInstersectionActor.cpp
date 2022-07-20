// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereRayInstersectionActor.h"

#include "MathUtil.h"

// Sets default values
ASphereRayInstersectionActor::ASphereRayInstersectionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
}

// Called when the game starts or when spawned
void ASphereRayInstersectionActor::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> children;
	GetAttachedActors(children);

	for (AActor* actor : children)
	{
		FString name = actor->GetActorLabel();
		if (name == TEXT("SR_Point_S"))
		{
			m_point_s = actor;
		}
		else if (name == TEXT("SR_Point_T"))
		{
			m_point_t = actor;
		}
		else if (name == TEXT("SR_Sphere"))
		{
			m_sphere = actor;
		}
	}
}

// Called every frame
void ASphereRayInstersectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_point_s == nullptr || m_point_t == nullptr || m_sphere == nullptr)
	{
		return;
	}

	DrawDebugLine(GetWorld(), m_point_s->GetActorLocation(), m_point_t->GetActorLocation(), FColor::Emerald, false, -1, 0, 10);

	FVector point_t = m_point_t->GetActorLocation();
	FVector point_s = m_point_s->GetActorLocation();
	FVector point_c = m_sphere->GetActorLocation();

	FVector s_to_t = point_t - point_s;
	FVector c_to_s = point_c - point_s;

	float s_to_t_length_squared = (s_to_t.Size() * s_to_t.Size());
	float s_to_t_dot_c_to_s = FVector::DotProduct(s_to_t, c_to_s);
	
	FVector point_p = point_s + (s_to_t * (s_to_t_dot_c_to_s / s_to_t_length_squared));
	FVector c_to_p = point_c - point_p;
	
	float c_to_p_length = c_to_p.Size();
	float c_to_p_length_squared = c_to_p_length * c_to_p_length;

	FVector orgin;
	FVector bounds_extent;
	m_sphere->GetActorBounds(false, orgin, bounds_extent);
	float sphere_radius = bounds_extent.X;

	if (c_to_p_length < sphere_radius)
	{
		float r_sqr = sphere_radius * sphere_radius;
		float distance_from_p = FMath::Sqrt(r_sqr - c_to_p_length_squared);

		FVector sp = point_p - point_s;
		FVector tp = point_p - point_t;

		// Not checking success of operation
		sp.Normalize(DBL_EPSILON);
		tp.Normalize(DBL_EPSILON);

		FVector intersection_one = point_p + (sp * distance_from_p);
		FVector intersection_two = point_p + (tp * distance_from_p);

		DrawDebugPoint(GetWorld(), intersection_one, 20, FColor::Red, false, -1, 0);
		DrawDebugPoint(GetWorld(), intersection_two, 20, FColor::Blue, false, -1, 0);
	}
}

