// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidControllerActor.h"

// Sets default values
ABoidControllerActor::ABoidControllerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidControllerActor::BeginPlay()
{
	Super::BeginPlay();

    for (int i= 0; i < 100; i++)
    {
        FVector random_location;
		random_location.X= FMath::FRandRange(0, 200);
		random_location.Y= FMath::FRandRange(0, 200);
		random_location.Z= FMath::FRandRange(0, 200);

		FRotator random_rotation;

		random_rotation.Pitch= FMath::FRandRange(-180.f, 180.f);
		random_rotation.Yaw= FMath::FRandRange(-180.f, 180.f);
		random_rotation.Roll= FMath::FRandRange(-180.f, 180.f);

		ABoidActor *boid= spawn_boid(random_location, random_rotation);
		boid->configure(0.2, 0.3, 0.4, 0.1, 0.1);
		m_boids.Add(boid);
    }
}

// Called every frame
void ABoidControllerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (ABoidActor *boid : m_boids)
	{
		boid->update_neighbors(m_boids);
	}
}

ABoidActor* ABoidControllerActor::spawn_boid(FVector location, FRotator rotation)
{
	FActorSpawnParameters spawn_params;
	ABoidActor* spawned_boid_reference= GetWorld()->SpawnActor<ABoidActor>(m_actor_to_spawn, location, rotation, spawn_params);
	return spawned_boid_reference;
}

