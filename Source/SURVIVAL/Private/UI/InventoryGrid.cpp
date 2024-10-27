// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryGrid.h"
#include "Character/RaiderCharacter.h"
#include "Components/InventoryComponent.h"
#include "UI/SlotGrid.h"
#include "Structures/STile.h"
#include "UI/BackgroundSlotGrid.h"

#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<ARaiderCharacter>(GetOwningPlayerPawn());
	check(GridPanel);
}

void UInventoryGrid::Refresh()
{
	if (CanvasPanel)
	{
		CanvasPanel->ClearChildren();

		TMap<UItemObject*, FSTile> Items;
		InventoryComponent->GetAllItems(Items);

		for (auto Item : Items) 
		{

			USlotGrid* ItemSlot = CreateWidget<USlotGrid>(GetWorld(), ItemSlotClass);

			ItemSlot->TileSize = TileSize;
			ItemSlot->ItemObject = Item.Key;
			ItemSlot->bIsLootInventory = bIsLootInventory;


			UPanelSlot* Widget = CanvasPanel->AddChild(ItemSlot);
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget);
			if (CanvasSlot)
			{
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetPosition(FVector2D(Item.Value.X * TileSize, Item.Value.Y * TileSize));
			}
		}
	}
}

void UInventoryGrid::GenerateBackgroundSlots()
{
	if (GridPanel)
	{
		GridPanel->ClearChildren();

		int32 Length = (InventoryComponent->Column * InventoryComponent->Rows);

		for (auto Index = 0; Index < Length; Index++)
		{
			UBackgroundSlotGrid* BackSlot = CreateWidget<UBackgroundSlotGrid>(GetWorld(), BackgroundSlotClass);

			BackSlot->TileSize = TileSize;

			int32 Rows = Index / InventoryComponent->Column;
			int32 Columns = Index % InventoryComponent->Column;

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
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GridBorder->Slot);
		if (CanvasSlot && InventoryComponent)
		{
			float Column = TileSize * InventoryComponent->Column;
			float Rows = TileSize * InventoryComponent->Rows;

			CanvasSlot->SetSize(FVector2D(Column, Rows));

			Refresh();

			InventoryComponent->OnInventoryChanged.AddUObject(this, &UInventoryGrid::Refresh);

			GenerateBackgroundSlots();
		}
	}
	
}





