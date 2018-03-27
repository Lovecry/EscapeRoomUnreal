// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Character = GetWorld()->GetFirstPlayerController()->GetPawn();
	Door = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlessurePlate->IsOverlappingActor(Character))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	float currentTime = GetWorld()->GetTimeSeconds();
	if (currentTime > (LastDoorOpenTime + DoorCloseDelay))
	{
		CloseDoor();
		LastDoorOpenTime = 0;
	}
}

void UOpenDoor::OpenDoor()
{
	Door->SetActorRotation(FRotator(0.0f, -angle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Door->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

