// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalPointer.h"
#include "Components/PointLightComponent.h"

// Sets default values
APortalPointer::APortalPointer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->CastShadow = false;

	PointLight = CreateDefaultSubobject<UPointLightComponent>("Point Light");
	PointLight->SetupAttachment(RootComponent);
	PointLight->CastShadows = false;

}

// Called when the game starts or when spawned
void APortalPointer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalPointer::DestroySelf()
{
	Destroy();
}

