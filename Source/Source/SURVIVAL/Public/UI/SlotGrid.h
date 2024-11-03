// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotGrid.generated.h"

class UDragWidget;
class UItemObject;
class USizeBox;
class UImage;
class UButton;
class UInventoryComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoved, UItemObject*);

UCLASS()
class SURVIVAL_API USlotGrid : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float TileSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UItemObject* ItemObject;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	bool bIsLootInventory;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	FOnRemoved OnRemoved;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UButton> SlotBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <USizeBox> SlotSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UImage> ImageIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseEnterColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseLeaveColor;
	UPROPERTY()
	FVector2D DragOffset;

	UPROPERTY(EditDefaultsOnly, Category = "DragDrop")
	TSubclassOf<UUserWidget> DragWidgetClass;

	virtual void NativeConstruct() override;
	//virtual void NativeOnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	//virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual  void NativeOnInitialized() override;
	
private:

	void Refresh() const;
};
