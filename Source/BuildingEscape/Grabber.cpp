// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandleComponent();
	FindInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector Location;
		FRotator Rotator;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotator);
		PhysicsHandle->SetTargetLocation(Location+Rotator.Vector() * Reach);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));


	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = hitResult.GetComponent();
	if (hitResult.GetActor())
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, hitResult.GetActor()->GetTransform().GetLocation(), true);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector Location;
	FRotator Rotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotator);

	FVector LineTraceEnd = Location + Rotator.Vector() * Reach;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByObjectType(Hit, Location, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
	}
	return Hit;
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent(); 

}

void UGrabber::FindInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		Input->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent Found : %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicHandleComponent Found : %s"), *GetOwner()->GetName());
	}
}
