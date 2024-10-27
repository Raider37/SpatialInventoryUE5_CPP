// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotGrid.h"
#include "Inventory/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Materials/MaterialInterface.h"

void USlotGrid::NativeConstruct()
{
	Refresh();
}

void USlotGrid::NativeOnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::OnMouseEnter(MyGeometry, MouseEvent);

	/*if (SlotBorder)
	{
		this->Widget->SetColorAndOpacity(MouseEnterColor);
	}*/
}

void USlotGrid::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::OnMouseLeave(MouseEvent);

	//this->Widget->SetColorAndOpacity(MouseLeaveColor);
}

void USlotGrid::Refresh()
{
	if (SlotSizeBox)
	{
		SlotSizeBox->SetWidthOverride(ItemObject->ItemInfo.Dimension.X * TileSize);
		SlotSizeBox->SetHeightOverride(ItemObject->ItemInfo.Dimension.Y * TileSize);

		UMaterialInstanceDynamic* Image = ImageIcon->GetDynamicMaterial();

		Image->SetTextureParameterValue("Icon", ItemObject->ItemInfo.Icon);

		if (ItemObject->ItemInfo.bIsRotatable)
		{
			Image->SetScalarParameterValue("RotationAngle(0 - 1)", -0.25f);
		}
		else
		{
			Image->SetScalarParameterValue("RotationAngle(0 - 1)", 0.0f);
		}
	}
}
