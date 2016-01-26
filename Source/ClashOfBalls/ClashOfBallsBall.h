// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "ClashOfBallsBall.generated.h"

UCLASS(config=Game)
class AClashOfBallsBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	AClashOfBallsBall();

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(Replicated, EditAnywhere, Category=Ball)
	float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(Replicated, EditAnywhere, Category=Ball)
	float RollTorque;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(Replicated, EditAnywhere, Category = Ball)
	bool bCanJump;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Health)
	float Health;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float MaxHealth = 100;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	float DamageMultiplier = 100;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action. */
	void Jump();

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	virtual void AddMovement_Implementation(bool ForwardBackward, float Value, FRotator Rotation);
	virtual bool AddMovement_Validate(bool ForwardBackward, float Value, FRotator Rotation);

	virtual void DoDamage_Implementation(AActor *Actor);
	virtual bool DoDamage_Validate(AActor *Actor);

	virtual void DoJump_Implementation();
	virtual bool DoJump_Validate();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	// add movement
	UFUNCTION(Server, Reliable, WithValidation)
	void AddMovement(bool ForwardBackward, float Value, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void DoJump();

	UFUNCTION(Server, Reliable, WithValidation)
	void DoDamage(AActor *Actor);

	// replicate attributes
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

};
