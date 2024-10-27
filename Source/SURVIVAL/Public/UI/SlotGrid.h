// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotGrid.generated.h"

class UItemObject;
class USizeBox;
class UImage;

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
	bool DragDropLocation;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <USizeBox> SlotSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UImage> ImageIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseEnterColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseLeaveColor;

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent);

private:

	void Refresh();
};
