// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryGrid;

UCLASS()
class SURVIVAL_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camponents", meta = (BindWidget))
	UInventoryGrid* Grid;

protected:
	
};
