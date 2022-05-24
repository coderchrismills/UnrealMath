// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/Engine/Classes/Components/SphereComponent.h>
#include "SphereRayInstersectionActor.generated.h"

UCLASS()
class UNREALMATH_API ASphereRayInstersectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphereRayInstersectionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor *m_point_s;
	AActor *m_point_t;
	AActor *m_sphere;
};
