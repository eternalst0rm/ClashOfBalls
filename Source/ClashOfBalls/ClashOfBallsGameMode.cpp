// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ClashOfBalls.h"
#include "ClashOfBallsGameMode.h"
#include "ClashOfBallsBall.h"

AClashOfBallsGameMode::AClashOfBallsGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = AClashOfBallsBall::StaticClass();
}
