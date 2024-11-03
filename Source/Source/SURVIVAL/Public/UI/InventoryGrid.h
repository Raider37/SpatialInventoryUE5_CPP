// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "InventoryGrid.generated.h"

class ARaiderCharacter;
class UInventoryComponent;
class UBorder;
class UGridPanel;
class USlotGrid;
class UItemObject;
class UBackgroundSlotGrid;
class UItemDragDropOperation;
class USlateBrushAsset;

UCLASS()
class SURVIVAL_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, Category = "Camponents")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UBorder> GridBorder;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UGridPanel> GridPanel;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UCanvasPanel> CanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	bool bIsLootInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USlotGrid> ItemSlotClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBackgroundSlotGrid> BackgroundSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "DropColor")
	FLinearColor GoodPlace;
	UPROPERTY(EditDefaultsOnly, Category = "DropColor")
	FLinearColor BadPlace;
	UPROPERTY(EditDefaultsOnly, Category = "DropColor")
	USlateBrushAsset* SlateBrushAsset;

	UPROPERTY()
	FIntPoint DraggedItemTopLeftIndex;
	UPROPERTY()
	UItemDragDropOperation* Operation;
	UPROPERTY()
	bool bDragDropLocation;

	void Initialization(UInventoryComponent* InvComp);
	void Refresh();
	void GenerateBackgroundSlots() const;

protected:

	UPROPERTY()
	UItemObject* CurrentItem;

	virtual void NativeConstruct() override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:

	FVector2D MousePosition;

	UPROPERTY()
	double TileSize;
	UPROPERTY()
	ARaiderCharacter* Character;

	

	void MousePostionInTile(FVector2D InMousePosition, bool& Right, bool& Down);
	bool IsRoomAvaliblePayload(UItemObject* Payload);
	void OnItemRemoved(UItemObject* ItemObject);
};
