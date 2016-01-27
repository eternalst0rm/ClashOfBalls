// Fill out your copyright notice in the Description page of Project Settings.

#include "ClashOfBalls.h"
#include "Net/UnrealNetwork.h"
#include "BasePlayerState.h"

// Called when the game starts or when spawned
void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();
	CalculateKDRatio();
}

// replicate attributes
void ABasePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlayerState, PlayerNumber);
	DOREPLIFETIME(ABasePlayerState, Kills);
	DOREPLIFETIME(ABasePlayerState, Deaths);
	DOREPLIFETIME(ABasePlayerState, KDRatio);

}

void ABasePlayerState::CalculateKDRatio_Implementation()
{
	if (Kills <= 0)
	{
		if (Deaths <= 0)
		{
			KDRatio = 1;
		}
		else
		{
			KDRatio = 1 / Deaths;
		}
	}
	else
	{
		if (Deaths <= 0)
		{
			KDRatio = Kills / 1;
		}
		else
		{
			KDRatio = Kills / Deaths;
		}
	}
}

bool ABasePlayerState::CalculateKDRatio_Validate()
{
	return true;
}

void ABasePlayerState::IncrementDeaths_Implementation()
{
	Deaths++;
	CalculateKDRatio();
}

bool ABasePlayerState::IncrementDeaths_Validate()
{
	return true;
}

void ABasePlayerState::IncrementKills_Implementation(bool WasFriendlyKilled)
{
	if (WasFriendlyKilled)
	{
		Kills--;
	}
	else
	{
		Kills++;
	}
	CalculateKDRatio();
}

bool ABasePlayerState::IncrementKills_Validate(bool WasFriendlyKilled)
{
	return true;
}


