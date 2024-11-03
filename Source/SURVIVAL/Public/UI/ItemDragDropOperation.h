// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/ItemObject.h"
#include "ItemDragDropOperation.generated.h"

class UItemObject;
class UInventoryComponent;
class APawn;

UCLASS()
class SURVIVAL_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DragOffset;

	UPROPERTY()
	UItemObject* ItemObject;

	UPROPERTY()
	bool bIsInventory;

	UPROPERTY()
	UInventoryComponent* InventoryComp;
	UPROPERTY()
	APawn* Owner;

	void GetPayload(UItemObject*& OutPayload);


protected:

	virtual void NativeOnDragCancelled(const FPointerEvent& PointerEvent);
};
