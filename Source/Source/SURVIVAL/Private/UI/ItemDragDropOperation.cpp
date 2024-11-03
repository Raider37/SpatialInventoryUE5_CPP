// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemDragDropOperation.h"
#include "Components/InventoryComponent.h"
#include "Inventory/ItemObject.h"



void UItemDragDropOperation::GetPayload(UItemObject*& OutPayload)
{
	OutPayload = Cast<UItemObject>(UDragDropOperation::Payload);
}

void UItemDragDropOperation::NativeOnDragCancelled(const FPointerEvent& PointerEvent)
{
	if (bIsInventory)
	{
		if (InventoryComp)
		{
			InventoryComp->TryAddItem(ItemObject);
		}
	}
}
