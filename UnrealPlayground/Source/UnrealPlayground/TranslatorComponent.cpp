// Fill out your copyright notice in the Description page of Project Settings.


#include "TranslatorComponent.h"

// Sets default values for this component's properties
UTranslatorComponent::UTranslatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTranslatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	///DebugDrawLine()...
	
}


// Called every frame
void UTranslatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

