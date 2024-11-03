// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryGrid.h"
#include "Character/RaiderCharacter.h"
#include "Components/InventoryComponent.h"
#include "Inventory/ItemObject.h"
#include "Structures/STile.h"
#include "UI/BackgroundSlotGrid.h"
#include "UI/ItemDragDropOperation.h"
#include "UI/SlotGrid.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<ARaiderCharacter>(GetOwningPlayerPawn());
	check(GridPanel);
}

void UInventoryGrid::MousePostionInTile(FVector2D InMousePosition, bool& Right, bool& Down)
{
	Right = (FMath::Fmod(InMousePosition.X, TileSize) > TileSize / 2.0);
	Down = (FMath::Fmod(InMousePosition.Y, TileSize) > TileSize / 2.0);

	//UKismetSystemLibrary::PrintString(GetWorld(), Right ? TEXT("Right: True") : TEXT("Right: False"), true, false, FLinearColor::Red, 0.f);
	//UKismetSystemLibrary::PrintString(GetWorld(), Down ? TEXT("Down: True") : TEXT("Down: False"), true, false, FLinearColor::Red, 0.f);

	return;
}

bool UInventoryGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Проверяем и вызываем родительский метод
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	if (InOperation)
	{
		Operation = Cast<UItemDragDropOperation>(InOperation);

		// Получаем позицию мыши в локальных координатах
		//MousePosition = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InDragDropEvent.GetScreenSpacePosition());

		MousePosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

		bool bRight;
		bool bDown;

		MousePostionInTile(MousePosition, bRight, bDown);

		UItemObject* Payload;
		Operation->GetPayload(Payload);

		int32 X;
		int32 Y;

		X = FMath::Clamp(Payload->ItemInfo.Dimension.X - (bRight ? 1 : 0), 0, Payload->ItemInfo.Dimension.X - (bRight ? 1 : 0));
		Y = FMath::Clamp(Payload->ItemInfo.Dimension.Y - (bDown ? 1 : 0), 0, Payload->ItemInfo.Dimension.Y - (bDown ? 1 : 0));

		FIntPoint IntPoint(X / 2, Y / 2);

		FIntPoint GridIndex;
		GridIndex.X = FMath::TruncToInt(MousePosition.X / TileSize);
		GridIndex.Y = FMath::TruncToInt(MousePosition.Y / TileSize);

		FIntPoint CurrDragIndex(FMath::TruncToInt(MousePosition.X / TileSize), FMath::TruncToInt(MousePosition.Y / TileSize));

		DraggedItemTopLeftIndex = CurrDragIndex - IntPoint;

		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("MousePosition: %s"), *MousePosition.ToString()), true, false, FLinearColor::Green, 0.f);
	}



	// Логгируем индексы для отладки
	UE_LOG(LogTemp, Display, TEXT("%d и %d"), DraggedItemTopLeftIndex.X, DraggedItemTopLeftIndex.Y);

	return true;
}

bool UInventoryGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Вызываем родительскую функцию
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (UItemDragDropOperation* DropOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		UItemObject* Item = nullptr;
		DropOperation->GetPayload(Item);

		if (Item && IsRoomAvaliblePayload(Item))  // Проверяем наличие Item и доступность места
		{
			int32 Index;
			// Конвертируем DraggedItemTopLeftIndex в индекс
			InventoryComponent->TileToIndex(DraggedItemTopLeftIndex.X, DraggedItemTopLeftIndex.Y, Index);

			// Добавляем элемент по рассчитанному индексу
			InventoryComponent->AddItemAt(Index, Item);

			UE_LOG(LogTemp, Display, TEXT("Item added at index %d"), Index);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Room unavailable or item is invalid."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop operation is invalid or not an item operation."));
	}

	return false;
}

//int32 UInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
//{
//	Super::NativePaint(Context);
//
//	if (UWidgetBlueprintLibrary::IsDragDropping() && bDragDropLocation)
//	{
//		UItemDragDropOperation* PaintOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::GetDragDroppingContent());
//
//		UItemObject* Item;
//		FLinearColor Color;
//
//		PaintOperation->GetPayload(Item);
//
//		Color = IsRoomAvaliblePayload(Item) ? GoodPlace : BadPlace;
//
//		FVector2D Position = DraggedItemTopLeftIndex * TileSize;
//		FVector2D Size = FVector2D(Item->ItemInfo.Dimension.X * TileSize, Item->ItemInfo.Dimension.Y * TileSize);
//
//		DrawBox(Context, Position, Size, SlateBrushAsset, Color);
//
//	}
//}

bool UInventoryGrid::IsRoomAvaliblePayload(UItemObject* Payload)
{
	if (Payload && InventoryComponent)
	{
		int32 Index;
		InventoryComponent->TileToIndex(DraggedItemTopLeftIndex.X, DraggedItemTopLeftIndex.Y, Index);
		if (InventoryComponent->IsRoomAvailable(Payload, Index))
		{
			return true;
		}
	}
	return false;

}

void UInventoryGrid::OnItemRemoved(UItemObject* ItemObject)
{
	InventoryComponent->RemoveItem(ItemObject);
}

void UInventoryGrid::Refresh()
	{
	if (CanvasPanel)
	{
		CanvasPanel->ClearChildren();

		TMap<UItemObject*, FSTile> Items;
		InventoryComponent->GetAllItems(Items);

		for (const auto Item : Items) 
		{

			USlotGrid* ItemSlot = CreateWidget<USlotGrid>(GetWorld(), ItemSlotClass);


			ItemSlot->TileSize = TileSize;
			ItemSlot->ItemObject = Item.Key;
			ItemSlot->bIsLootInventory = bIsLootInventory;
			ItemSlot->InventoryComponent = InventoryComponent;

			ItemSlot->OnRemoved.AddUObject(this, &UInventoryGrid::OnItemRemoved);

			UPanelSlot* Widget = CanvasPanel->AddChild(ItemSlot);
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget))
			{
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetPosition(FVector2D(Item.Value.X * TileSize, Item.Value.Y * TileSize));
			}
		}
	}
}

void UInventoryGrid::GenerateBackgroundSlots() const
{
	if (GridPanel)
	{
		GridPanel->ClearChildren();

		const int32 Length = (InventoryComponent->Column * InventoryComponent->Rows);

		for (auto Index = 0; Index < Length; Index++)
		{
			UBackgroundSlotGrid* BackSlot = CreateWidget<UBackgroundSlotGrid>(GetWorld(), BackgroundSlotClass);

			BackSlot->TileSize = TileSize;

			const int32 Rows = Index / InventoryComponent->Column;
			const int32 Columns = Index % InventoryComponent->Column;

			GridPanel->AddChildToGrid(BackSlot, Rows, Columns);
		}
	}
}

void UInventoryGrid::Initialization(UInventoryComponent* InvComp)
{
 	InventoryComponent = InvComp;

	TileSize = InventoryComponent->TileSize;

	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is nullptr"));
		return;
	}

	if (!this)
	{
		UE_LOG(LogTemp, Warning, TEXT("this is nullptr in Initialization"));
		return;
	}

	if (GridBorder)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot); CanvasSlot && InventoryComponent)
		{
			const double Column = TileSize * InventoryComponent->Column;
			const double Rows = TileSize * InventoryComponent->Rows;

			CanvasSlot->SetSize(FVector2D(Column, Rows));
			GenerateBackgroundSlots();
			Refresh();

			InventoryComponent->OnInventoryChanged.AddUObject(this, &UInventoryGrid::Refresh);
		}
	}
	
}





