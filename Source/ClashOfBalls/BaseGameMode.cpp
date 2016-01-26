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

	TArray<AActor*> listPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallController::StaticClass(), listPlayer);

	for (AActor* i : listPlayer)
	{
		ABallController *PC = Cast <ABallController>(i);
		if (PC != NULL)
		{
			if (PC->IsDead == true)
			{
				bool foundSpawn = false;
				int32 spawnPos = 0;
				while (foundSpawn == false)
				{
					int32 pos = FMath::RandRange(0, ListRespawn.Num() - 1);
					if (ListRespawn[pos]->IsBlocked == false)
					{
						foundSpawn = true;
						spawnPos = pos;
					}
				}


				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				AClashOfBallsBall *Spawner = GetWorld()->SpawnActor<AClashOfBallsBall>(DefaultPawnClass, ListRespawn[spawnPos]->GetActorLocation(), ListRespawn[spawnPos]->GetActorRotation(), SpawnParams);
				if (Spawner)
				{
					PC->Possess(Spawner);
					PC->IsDead = false;
				}
			}
		}
	}
}