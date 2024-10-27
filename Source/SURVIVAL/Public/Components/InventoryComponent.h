// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/Stile.h"
#include "Containers/Map.h"
#include "InventoryComponent.generated.h"

class UItemObject;
class ABaseInventoryItem;
class ARaiderCharacter;
class UInventoryWidget;

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<UItemObject*> Inventory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Column;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Rows;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float TileSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidget;
	FOnInventoryChanged OnInventoryChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryAddItem(UItemObject* ItemToAdd);
	bool IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex);
	void AddItemAt(int32 TopLeftIndex, UItemObject* ItemObject);
	bool CheckCanSplitItem(UItemObject* ItemToAdd);
	void GetAllItems(TMap<UItemObject*, FSTile>& Items);
	void RemoveItem(UItemObject* ItemObject);

private:
	UPROPERTY()
	ARaiderCharacter* Character;

	bool AddAmount(UItemObject* NewItem, UItemObject* OldItem, int32& Amount);
	void IndexToTile(int32 Index, int32& X, int32& Y);
	void TileToIndex(int32 X, int32 Y, int32& Index);
	void ForEachIndex(int32 TopLeftIndex, UItemObject* ItemObject, TArray<FIntPoint>& OutIndices);
	bool IsTileValid(int32 X, int32 Y);
	bool GetItemAtIndex(int32 Index, UItemObject*& ItemObject);

};
