// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Inventory/BaseInventoryItem.h"
#include "Inventory/ItemObject.h"
#include "Character/RaiderCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TileSize = 60;
	Column = 0;
	Rows = 0;
	// ...
	AActor* Owner = GetOwner();
	Character = Cast<ARaiderCharacter>(Owner);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(Column * Rows);
	AActor* Owner = GetOwner();
	Character = Cast<ARaiderCharacter>(Owner);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddAmount(const UItemObject* NewItem, const UItemObject* OldItem, int32& Amount)
{
	Amount = NewItem->ItemInfo.Amount + OldItem->ItemInfo.Amount;

	if (NewItem->ItemInfo.UsableClass == OldItem->ItemInfo.UsableClass && Amount < OldItem->ItemInfo.MaxAmount)
	{
		return true;
	}
	Amount = OldItem->ItemInfo.Amount;
	return false;
}

void UInventoryComponent::IndexToTile(int32 Index, int32& X, int32& Y) const
{
	X = Index % Column;
	Y = Index / Column;
	return;
}

void UInventoryComponent::TileToIndex(int32 X, int32 Y, int32& Index) const
{
	Index = (Column * Y) + X;
	return;
}

bool UInventoryComponent::IsTileValid(int32 X, int32 Y) const
{
	if (X >= 0 && Y >= 0 && X < Column && Y < Rows)
	{
		return true;
	}
	return false;
}

bool UInventoryComponent::GetItemAtIndex(const int32 Index, UItemObject*& ItemObject)
{
	if (Inventory.IsValidIndex(Index))
	{
		ItemObject = Inventory[Index];
		return true;
	}
	ItemObject = nullptr;
	return false;
}

void UInventoryComponent::ForEachIndex(const int32 TopLeftIndex, const UItemObject* ItemObject, TArray<FIntPoint>& OutIndices) const
{
	if (ItemObject != nullptr)
	{
		int32 TileX = 0;
		int32 TileY = 0;

		IndexToTile(TopLeftIndex, TileX, TileY);

		for (int32 IndexX = TileX; IndexX < TileX + ItemObject->ItemInfo.Dimension.X; IndexX++)
		{
			for (int32 IndexY = TileY; IndexY < TileY + ItemObject->ItemInfo.Dimension.Y; IndexY++)
			{
				OutIndices.Add(FIntPoint(IndexX, IndexY));
			}
		}
	}
}

bool UInventoryComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex)
{
	if (ItemObject)
	{
		TArray<FIntPoint> Indices;
		ForEachIndex(TopLeftIndex, ItemObject, Indices);

			for (const FIntPoint& Point : Indices)
			{
				if (!IsTileValid(Point.X, Point.Y))
				{
					return false;
				}

				int32 Index;
				TileToIndex(Point.X, Point.Y, Index);
				UItemObject* ReturnItemObject;
				if (GetItemAtIndex(Index, ReturnItemObject))
				{
					if (IsValid(ReturnItemObject))
					{
						return false;
					}
				}
			}
		return true;
	}
	return false;
}

void UInventoryComponent::AddItemAt(const int32 TopLeftIndex, UItemObject* ItemObject)
{
	TArray<FIntPoint> Indices;
	ForEachIndex(TopLeftIndex, ItemObject, Indices);

	for (const FIntPoint& Point : Indices)
	{
		int32 Index;
		TileToIndex(Point.X, Point.Y, Index);
		Inventory[Index] = ItemObject; 
	}
	OnInventoryChanged.Broadcast();
}

bool UInventoryComponent::CheckCanSplitItem(const UItemObject* ItemToAdd)
{
	if (ItemToAdd && ItemToAdd->ItemInfo.MaxAmount > 1 && ItemToAdd->ItemInfo.bIsStack)
	{
		TArray<FIntPoint> OutIndices;
		for (auto Index = 0; Index < Inventory.Num(); Index++)
		{
			ForEachIndex(Index, ItemToAdd, OutIndices);
		}

		for (FIntPoint Point : OutIndices)
		{
			
			if (!IsTileValid(Point.X, Point.Y))
			{
				return false;
			}
			int32 Index;
			TileToIndex(Point.X, Point.Y, Index);
			if (IsValid(Inventory[Index]))
			{
				int32 Amount;
				if (AddAmount(ItemToAdd, Inventory[Index], Amount))
				{
					Inventory[Index]->ItemInfo.Amount = Amount;

					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryComponent::GetAllItems(TMap<UItemObject*, FSTile>& Items)
{
	for (auto Index = 0; Index < Inventory.Num(); Index++)
	{
		if (Inventory[Index])
		{
			UItemObject* CurrentItemObject = Inventory[Index];
			if (!Items.Contains(CurrentItemObject))
			{
				int32 TileX;
				int32 TileY;
				IndexToTile(Index, TileX, TileY);

				FSTile Tile;
				Tile.X = TileX;
				Tile.Y = TileY;

				Items.Add(CurrentItemObject, Tile);
			}
		}
	}
}

void UInventoryComponent::RemoveItem(const UItemObject* ItemObject)
{
	if (ItemObject)
	{
		for (auto Index = 0; Index < Inventory.Num(); Index++)
		{
			if (ItemObject == Inventory[Index])
			{
				Inventory[Index] = nullptr;

				OnInventoryChanged.Broadcast();
			}
		}
	}
}

bool UInventoryComponent::AddStackToSlot(UItemObject* NewItem, UItemObject* OldItem)
{
	if (NewItem && OldItem)
	{
		int32 Amount;
		if (AddAmount(NewItem, OldItem, Amount)) 
		{
			OldItem->ItemInfo.Amount = Amount;
			OnInventoryChanged.Broadcast();
			return true;
		}
	}
	else
	{
		if (TryAddItem(NewItem))
		{
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::TryAddItem(UItemObject* ItemToAdd)
	{
	if (IsValid(ItemToAdd))
	{
		for (auto Index = 0; Index < Inventory.Num(); Index++)
		{
			if (IsRoomAvailable(ItemToAdd, Index))
			{
				AddItemAt(Index, ItemToAdd);
				OnInventoryChanged.Broadcast();
				UE_LOG(LogTemp, Display, TEXT("%d"), Index);
				return true;
			}
		}
		////////////// rotate logic

	}
	return false; 
}
