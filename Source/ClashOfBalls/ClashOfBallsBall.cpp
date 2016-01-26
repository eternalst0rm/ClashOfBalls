// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ClashOfBalls.h"
#include "Net/UnrealNetwork.h"
#include "BallController.h"
#include "ClashOfBallsBall.h"

AClashOfBallsBall::AClashOfBallsBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	Health = MaxHealth;
}

// replicate attributes
void AClashOfBallsBall::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClashOfBallsBall, RollTorque);
	DOREPLIFETIME(AClashOfBallsBall, JumpImpulse);
	DOREPLIFETIME(AClashOfBallsBall, bCanJump);
	DOREPLIFETIME(AClashOfBallsBall, Health);
	DOREPLIFETIME(AClashOfBallsBall, MaxHealth);
	DOREPLIFETIME(AClashOfBallsBall, DamageMultiplier);
}

void AClashOfBallsBall::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveRight", this, &AClashOfBallsBall::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &AClashOfBallsBall::MoveForward);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AClashOfBallsBall::Jump);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AClashOfBallsBall::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AClashOfBallsBall::LookUpAtRate);
}

void AClashOfBallsBall::MoveRight(float Val)
{
	AddMovement(false, Val, Camera->GetComponentRotation());
}

void AClashOfBallsBall::MoveForward(float Val)
{
	AddMovement(true, Val, Camera->GetComponentRotation());
}

void AClashOfBallsBall::AddMovement_Implementation(bool ForwardBackward, float Value, FRotator Rotation)
{
	if (ForwardBackward)
	{
		const FVector Torque = Rotation.RotateVector(FVector(0, Value * RollTorque, 0.f));
		Ball->AddTorque(Torque);
	}
	else
	{
		const FVector Torque = Rotation.RotateVector(FVector(-1 * Value * RollTorque, 0.f, 0.f));
		Ball->AddTorque(Torque);
	}
}

bool AClashOfBallsBall::AddMovement_Validate(bool ForwardBackward, float Value, FRotator Rotation)
{
	return true;
}

void AClashOfBallsBall::Jump()
{
	DoJump();
}

void AClashOfBallsBall::DoJump_Implementation()
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

bool AClashOfBallsBall::DoJump_Validate()
{
	return true;
}

// turn rate
void AClashOfBallsBall::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// look up rate
void AClashOfBallsBall::LookUpAtRate(float Rate)
{
	//UE_LOG(LogTemp, Warning, TEXT("LookUp/Down"));
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AClashOfBallsBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;

	DoDamage(Other);
}

void AClashOfBallsBall::DoDamage_Implementation(AActor *Actor)
{
	AClashOfBallsBall *enemy = Cast<AClashOfBallsBall>(Actor);
	if (enemy)
	{
		float dmg = (GetVelocity().X + GetVelocity().Y + GetVelocity().Z) / DamageMultiplier;
		if (dmg < 0)
		{
			dmg *= -1;
		}
		enemy->TakeDamage(dmg, FDamageEvent(), this->GetInstigatorController(), this);
	}
}

bool AClashOfBallsBall::DoDamage_Validate(AActor *Actor)
{
	return true;
}

float AClashOfBallsBall::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	float ActualDamage = 0;

	if (Health > 0)
	{
		ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		if (ActualDamage > 0)
		{
			Health -= ActualDamage;
			if (Health <= 0)
			{
				// kill player and start respawn
				ABallController *controller = Cast<ABallController>(this->GetController());
				if (controller)
				{
					controller->IsDead = true;
					if (DamageCauser)
					{
						controller->KilledBy = DamageCauser;
					}
					// spawn death point

					controller->StartRespawn();
					controller->UnPossess();
					this->Destroy();
				}
			}
		}
	}
	return ActualDamage;
}