// Fill out your copyright notice in the Description page of Project Settings.

#include "ClashOfBalls.h"
#include "Net/UnrealNetwork.h"
#include "ClashOfBallsBall.h"
#include "SpawnPoint.h"
#include "BaseGameMode.h"
#include "BasePlayerState.h"
#include "BallController.h"

// replicate attributes
void ABallController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABallController, IsDead);
	DOREPLIFETIME(ABallController, KilledBy);
}

// Called every frame
void ABallController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABallController::AddDeath_Implementation()
{
	ABasePlayerState *playerState = Cast<ABasePlayerState>(this->PlayerState);
	if (playerState)
	{
		playerState->IncrementDeaths();
	}
}

bool ABallController::AddDeath_Validate()
{
	return true;
}

void ABallController::NotifyKiller_Implementation(bool friendly)
{
	if (KilledBy)
	{
		AClashOfBallsBall *enemy = Cast<AClashOfBallsBall>(KilledBy);
		if (enemy)
		{
			ABasePlayerState *playerState = Cast<ABasePlayerState>(enemy->PlayerState);
			if (playerState)
			{
				playerState->IncrementKills(friendly);
			}
		}
	}
}

bool ABallController::NotifyKiller_Validate(bool friendly)
{
	return true;
}

// respawn player
void ABallController::Respawn_Implementation()
{
	if (this)
	{
		int counter = 0;
		ABaseGameMode *gameMode = (ABaseGameMode*)GetWorld()->GetAuthGameMode();
		if (gameMode)
		{
			while (IsDead)
			{
				TArray<ASpawnPoint*> ListRespawn = gameMode->ListRespawn;

				bool foundSpawn = false;
				int32 spawnPos = 0;
				while (foundSpawn == false)
				{
					if (counter > 5)
					{
						foundSpawn = true;
						spawnPos = 0;
					}

					int32 pos = FMath::RandRange(0, ListRespawn.Num() - 1);
					if (ListRespawn[pos]->IsBlocked == false)
					{
						foundSpawn = true;
						spawnPos = pos;
					}

					counter++;
				}

				//ListRespawn[spawnPos]->Mesh->rotation

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				AClashOfBallsBall *Spawner = GetWorld()->SpawnActor<AClashOfBallsBall>(gameMode->DefaultPawnClass, ListRespawn[spawnPos]->GetActorLocation(), ListRespawn[spawnPos]->GetActorRotation(), SpawnParams);


				if (Spawner)
				{
					IsDead = false;
					Spawner->SetActorRotation(ListRespawn[spawnPos]->GetActorRotation());
					this->Possess(Spawner);
				}
			}
		}
	}
}

bool ABallController::Respawn_Validate()
{
	return true;
}

void ABallController::StartRespawn_Implementation()
{
	GetWorldTimerManager().SetTimer(RespawnHandle, this, &ABallController::Respawn, 5, false);
}

bool ABallController::StartRespawn_Validate()
{
	return true;
}


