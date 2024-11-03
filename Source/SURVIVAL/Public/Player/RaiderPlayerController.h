// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "RaiderPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ARaiderPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputMapping")
	UInputMappingContext* InputMappingContext;
	
	virtual void BeginPlay() override;
};
