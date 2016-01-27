// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BallController.generated.h"

/**
 * 
 */
UCLASS()
class CLASHOFBALLS_API ABallController : public APlayerController
{
	GENERATED_BODY()
	
	FTimerHandle RespawnHandle;

	FTimerHandle DeathPointHandle;

	virtual void AddDeath_Implementation();

	virtual bool AddDeath_Validate();

	virtual void NotifyKiller_Implementation(bool friendly);

	virtual bool NotifyKiller_Validate(bool friendly);

	virtual void Respawn_Implementation();

	virtual bool Respawn_Validate();

	virtual void StartRespawn_Implementation();

	virtual bool StartRespawn_Validate();

public:

	//############################################################ functions

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// replicate attributes
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	// respawn player
	UFUNCTION(Server, Reliable, WithValidation)
		void Respawn();

	UFUNCTION(Server, Reliable, WithValidation)
		void StartRespawn();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Notify")
		void AddDeath();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Notify")
		void NotifyKiller(bool friendly);

	// ######################################################### flag for IsDead

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Death)
		bool IsDead = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Score)
		class AActor* KilledBy;
};
