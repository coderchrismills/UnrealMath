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
	
}

// Called every frame
void ABoidControllerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

