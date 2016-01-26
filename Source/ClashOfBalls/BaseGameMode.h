// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLASHOFBALLS_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// list of all RespawnPoints of the map
	TArray<class ASpawnPoint*> ListRespawn;
	
	
};
