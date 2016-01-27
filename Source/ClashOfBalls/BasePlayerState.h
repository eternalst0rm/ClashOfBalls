// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CLASHOFBALLS_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Stats)
		int32 PlayerNumber = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
		float Kills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
		float Deaths = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Stats)
		float KDRatio = 0.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CalculateKDRatio_Implementation();

	virtual bool CalculateKDRatio_Validate();

	virtual void IncrementDeaths_Implementation();

	virtual bool IncrementDeaths_Validate();

	virtual void IncrementKills_Implementation(bool WasFriendlyKilled);

	virtual bool IncrementKills_Validate(bool WasFriendlyKilled);

	// replicate attributes
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	UFUNCTION(Server, Reliable, WithValidation)
		void IncrementKills(bool WasFriendlyKilled);

	UFUNCTION(Server, Reliable, WithValidation)
		void IncrementDeaths();

	UFUNCTION(Server, Reliable, WithValidation)
		void CalculateKDRatio();
	
	
};
