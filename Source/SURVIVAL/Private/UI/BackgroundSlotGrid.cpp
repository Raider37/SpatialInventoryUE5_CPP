// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BackgroundSlotGrid.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"

void UBackgroundSlotGrid::Refresh()
{
	if (SizeBoxSlot)
	{
		SizeBoxSlot->SetWidthOverride(TileSize);
		SizeBoxSlot->SetHeightOverride(TileSize);
	}
	
}

void UBackgroundSlotGrid::NativePreConstruct()
{
	Super::NativePreConstruct();
	Refresh();
}

void UBackgroundSlotGrid::NativeConstruct()
{
	
}

void UBackgroundSlotGrid::NativeOnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseEnter called."));
	Super::OnMouseEnter(MyGeometry, MouseEvent);

	if (SlotBorder)
	{
		SlotBorder->SetColorAndOpacity(MouseEnterColor);
	}
}
 
void UBackgroundSlotGrid::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::OnMouseLeave(MouseEvent);

	SlotBorder->SetColorAndOpacity(MouseLeaveColor);
}


