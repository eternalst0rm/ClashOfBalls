// Fill out your copyright notice in the Description page of Project Settings.

#include "ClashOfBalls.h"
#include "SpawnPoint.h"
#include "BallController.h"
#include "ClashOfBallsBall.h"
#include "BaseGameMode.h"

// Called when the game starts or when spawned
void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	//############################################### spawn player at beginning of game

	TArray<AActor*> list;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), list);

	for (AActor* i : list)
	{
		ListRespawn.Add(Cast<ASpawnPoint>(i));
	}
}