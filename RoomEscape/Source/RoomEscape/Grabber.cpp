// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    
    // Look for attached Physics Handle
    FindPhysicsHandleComponent();
    
    // Look for attached input componnet (Run time only)
    BindInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        // Physics Handle found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s' misses physics handle component."), *GetOwner()->GetName());
    }
}

void UGrabber::BindInputComponent()
{
    InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input Component found"));
        
        // Bind the input with the input panel
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("'%s' misses input component."), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));
    
    // Try and reach any actor with physics body collision channel set
    GetFirstPhysicsBodyInReach();
    // If we hit something then attach a physics handle
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    if (ActorHit != nullptr)
    {
        // Grab phsics handle
        PhysicsHandle -> GrabComponentAtLocation(
            ComponentToGrab,
            NAME_None,
            ComponentToGrab -> GetOwner() -> GetActorLocation()
        );
    }
    
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Release key pressed"));
    
    // TODO: Release phsics handle
    PhysicsHandle -> ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    // Get Player View Point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
       OUT PlayerViewPointLocation,
       OUT PlayerViewPointRotation
    );
    /// Ray-cast out to reach distance
    //    UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
    
    /// Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    /// Draw a red debug line to visualize the sight of the pawn
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
    
    /// Line-trace (AKA ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld() -> LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
    
    /// See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit object: %s"), *(ActorHit->GetName()))
    }
    
    return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Get Player View Point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
       OUT PlayerViewPointLocation,
       OUT PlayerViewPointRotation
    );
    /// Ray-cast out to reach distance
    //    UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
    
    /// Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    /// Draw a red debug line to visualize the sight of the pawn
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    // If the phsics handle is attached, move the object that we are holding
    if (PhysicsHandle -> GrabbedComponent)
    {
        PhysicsHandle -> SetTargetLocation(LineTraceEnd);
    }
	
}

