// Fill out your copyright notice in the Description page of Project Settings.


#include "NearestPointToLineActor.h"

// Sets default values
ANearestPointToLineActor::ANearestPointToLineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
}

// Called when the game starts or when spawned
void ANearestPointToLineActor::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> children;
	GetAttachedActors(children);

	for (AActor* actor : children)
	{
		FString name = actor->GetActorLabel();
		if (name == TEXT("NPL_Point_P"))
		{
			m_point_p = actor;
		}
		else if (name == TEXT("NPL_Point_S"))
		{
			m_point_s = actor;
		}
		else if (name == TEXT("NPL_Point_T"))
		{
			m_point_t = actor;
		}
	}
}

// Called every frame
void ANearestPointToLineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_point_s == nullptr || m_point_t == nullptr || m_point_p == nullptr)
	{
		return;
	}
	
	FVector point_s = m_point_s->GetActorLocation();
	FVector point_t = m_point_t->GetActorLocation();
	FVector point_p = m_point_p->GetActorLocation();

	// line between s and t
	DrawDebugLine(GetWorld(), point_s, point_t, FColor::Emerald, false, -1, 0, 10);

	FVector s_to_t = point_t - point_s;
	FVector s_to_p = point_p - point_s;
	float s_to_t_length_squared = (s_to_t.Size() * s_to_t.Size());
	float s_to_t_dot_s_to_p = FVector::DotProduct(s_to_t, s_to_p) / s_to_t_length_squared;
	FVector intersection = point_s + (s_to_t * s_to_t_dot_s_to_p);

	if (s_to_t_dot_s_to_p <= 0)
	{
		DrawDebugLine(GetWorld(), point_s, point_p, FColor::Blue, false, -1, 0, 10);
	}
	else if (s_to_t_dot_s_to_p >= 1)
	{
		DrawDebugLine(GetWorld(), point_t, point_p, FColor::Blue, false, -1, 0, 10);
	}
	else
	{
		DrawDebugLine(GetWorld(), point_s, intersection, FColor::Red, false, -1, 0, 20);
		DrawDebugLine(GetWorld(), intersection, point_p, FColor::Blue, false, -1, 0, 10);
	}
}
