// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalPointer.generated.h"

UCLASS()
class FPSSURVIVAL_API APortalPointer : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	class UPointLightComponent* PointLight;
	
public:	
	// Sets default values for this actor's properties
	APortalPointer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void DestroySelf();


};
