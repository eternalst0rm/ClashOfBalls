// Fill out your copyright notice in the Description page of Project Settings.

#include "ClashOfBalls.h"
#include "Net/UnrealNetwork.h"
#include "ClashOfBallsBall.h"
#include "SpawnPoint.h"


// Sets default values
ASpawnPoint::ASpawnPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RespawnPoint"));
	RootComponent = Mesh;

	IsBlockedVolume = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("IsBlockedVolume"));
	IsBlockedVolume->InitCapsuleSize(8.f, 5.f);
	IsBlockedVolume->AttachParent = Mesh;
}

// replicate attributes
void ASpawnPoint::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnPoint, IsBlocked);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TArray<AActor*> overlappingActors;

	IsBlockedVolume->GetOverlappingActors(overlappingActors, AClashOfBallsBall::StaticClass());

	if (overlappingActors.Num() > 0)
	{
		IsBlocked = true;
	}
	else
	{
		IsBlocked = false;
	}
}

