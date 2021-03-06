// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    // Ray-cast, reach and grab
    virtual void Grab();
    
    virtual void Release();
    
    virtual FVector GetReachLineStart();
    
    virtual FVector GetReachLineEnd();

private:
    // How far can the player reach in cm
    UPROPERTY(EditAnywhere)
    float Reach = 80.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    
    UInputComponent* InputComponent = nullptr;
    
    virtual void FindPhysicsHandleComponent();
    
    virtual void BindInputComponent();
    
    const FHitResult GetFirstPhysicsBodyInReach();
	
};
