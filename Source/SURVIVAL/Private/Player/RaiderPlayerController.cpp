// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RaiderPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ARaiderPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
