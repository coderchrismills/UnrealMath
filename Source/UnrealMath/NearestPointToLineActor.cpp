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
		if (name == TEXT("Point_P"))
		{
			m_point_p = actor;
		}
		else if (name == TEXT("Point_S"))
		{
			m_point_s = actor;
		}
		else if (name == TEXT("Point_T"))
		{
			m_point_t = actor;
		}
	}
}

// Called every frame
void ANearestPointToLineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//https://docs.unrealengine.com/5.0/en-US/API/Runtime/Engine/DrawDebugPoint/
	if (m_point_s != nullptr && m_point_t != nullptr)
	{
		// line between s and t
		DrawDebugLine(GetWorld(), m_point_s->GetActorLocation(), m_point_t->GetActorLocation(), FColor::Emerald, false, -1, 0, 10);

		// compute normal between s-t and p.
		FVector st = m_point_t->GetActorLocation() - m_point_s->GetActorLocation();
		FVector sp = m_point_p->GetActorLocation() - m_point_s->GetActorLocation();
		float st_sq = (st.Size() * st.Size());
		float proj_st_sp = FVector::DotProduct(st, sp) / st_sq;
		FVector proj_on_st = m_point_s->GetActorLocation() + (st * proj_st_sp);

		if (proj_st_sp <= 0) 
		{
			DrawDebugLine(GetWorld(), m_point_s->GetActorLocation(), m_point_p->GetActorLocation(), FColor::Blue, false, -1, 0, 10);
		}
		else if (proj_st_sp >= 1)
		{
			DrawDebugLine(GetWorld(), m_point_t->GetActorLocation(), m_point_p->GetActorLocation(), FColor::Blue, false, -1, 0, 10);
		}
		else
		{
			DrawDebugLine(GetWorld(), m_point_s->GetActorLocation(), proj_on_st, FColor::Red, false, -1, 0, 20);
			DrawDebugLine(GetWorld(), proj_on_st, m_point_p->GetActorLocation(), FColor::Blue, false, -1, 0, 10);
		}
	}
}

void ANearestPointToLineActor::spawn_point_s()
{
	
}

void ANearestPointToLineActor::spawn_point_t()
{
	
}

void ANearestPointToLineActor::spawn_point_p()
{
	/*
	FActorSpawnParameters spawn_params;
	FString name = TEXT("Point-P");
	AActor* spawned = GetWorld()->SpawnActor<AActor>(m_point_p, FRotator::ZeroRotator, spawn_params);
	spawned->SetActorLabel(name);
	spawned->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);

	m_point_p_mesh_component = NewObject<UStaticMeshComponent>(spawned);
	m_point_p_mesh_component->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	m_point_p_mesh_component->RegisterComponent();
	m_point_p_mesh_component->SetStaticMesh(m_point_t_mesh);
	m_point_p_mesh_component->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	*/
}
