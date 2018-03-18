// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

// For using BlueprintAssignable
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    virtual void OpenDoor();
    virtual void CloseDoor();
    
    UPROPERTY(BlueprintAssignable)
    FOnOpenRequest OnOpenRequest;

private:
    UPROPERTY(EditAnywhere)
    float OpenAngle = -60.f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    
    UPROPERTY(EditAnywhere)
    float TriggerMass = 40.f;
    
    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = .5f;
    
    float LastDoorOpenTime;
    
    AActor* Owner = nullptr; // who owns the door
    
    float GetTotalMassOfActorsOnPlate();
	
};
