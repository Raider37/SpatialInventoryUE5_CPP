// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STile.generated.h"

USTRUCT(BlueprintType)
struct FSTile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;


	FSTile()
		: X(), Y()
	{}
};
