// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackgroundSlotGrid.generated.h"

class USizeBox;
class UButton;

UCLASS()
class SURVIVAL_API UBackgroundSlotGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <USizeBox> SizeBoxSlot;

	UPROPERTY()
	float TileSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseEnterColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FLinearColor MouseLeaveColor;

protected:

	virtual void NativePreConstruct() override;

	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UButton> SlotBorder;

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent);

private:

	void Refresh();

};
