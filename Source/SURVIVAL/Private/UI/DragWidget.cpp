// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragWidget.h"
#include "Inventory/ItemObject.h"
#include "Character/RaiderCharacter.h"
#include "Components/InventoryComponent.h"

void UDragWidget::NativeConstruct()
{
	ARaiderCharacter* Character = Cast<ARaiderCharacter>(GetOwningPlayerPawn());
	TileSize = Character->InventoryComponent->TileSize;
}

void UDragWidget::SetImageDragDropItem(FSlateBrush& Brush)
{
	if (ItemObject)
	{
		Brush.SetResourceObject(ItemObject->ItemInfo.Icon);
		Brush.ImageSize = FVector2D(ItemObject->ItemInfo.Dimension.X * TileSize, ItemObject->ItemInfo.Dimension.Y * TileSize);
	}
}
